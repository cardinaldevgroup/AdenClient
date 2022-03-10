#include "GameBlockAllocator.h"

#include <string.h>

// ����������ڴ��С
static const int s_nChunkSize = 16 * 1024;

// ����������ʱ���ο��ٵ���������
static const int s_nChunkIncrement = 128;

// ��ͬ�ڴ���С��������������
static const int s_nChunkSizeCount = 5;

// ��������������ڴ���С
static const int s_arrBlockSize[s_nChunkSizeCount] = {
	16, 32, 64, 128, 256
};

// �ڴ�����ߴ�
static const int s_nMaxBlockSize = 256;

class GameBlockAllocator::Impl
{
public:
	struct Block
	{
		Block*	pNext;
	};
	struct ChunkInfo
	{
		int		nChunkSize;
		Block*	pBlocks;
	};

	int			m_nChunkCount;
	int			m_nChunkSpace;
	ChunkInfo*	m_arrChunkInfo;

	int			m_arrSizeMap[s_nMaxBlockSize + 1];
	Block*		m_arrFreeBlockHead[s_nChunkSizeCount];

public:
	Impl()
	{
		m_nChunkCount = 0;
		m_nChunkSpace = s_nChunkIncrement;
		m_arrChunkInfo = new ChunkInfo[m_nChunkSpace];

		memset(m_arrChunkInfo, 0, m_nChunkSpace * sizeof(Block));
		memset(m_arrFreeBlockHead, 0, sizeof(m_arrFreeBlockHead));

		m_arrSizeMap[0] = 0;
		int nCurrentSizeIndex = 0;
		for (int i = 1; i <= s_nMaxBlockSize; i++)
		{
			if (i > s_arrBlockSize[nCurrentSizeIndex]) {
				nCurrentSizeIndex++;
			}
			m_arrSizeMap[i] = nCurrentSizeIndex;
		}
	}
	~Impl()
	{
		for (int i = 0; i < m_nChunkCount; i++)
		{
			delete[] m_arrChunkInfo[i].pBlocks;
		}
		delete[] m_arrChunkInfo;
	}
};

void* GameBlockAllocator::Allocate(size_t nSize)
{
	if (nSize == 0)
	{
		return nullptr;
	}

	if (nSize > s_nMaxBlockSize)
	{
		return new char[nSize];
	}

	// �ҵ��ʺϸö������������
	int index = m_pImpl->m_arrSizeMap[nSize];

	// ������������п���λ�ã���ɷ�����ָ��
	if (m_pImpl->m_arrFreeBlockHead[index])
	{
		Impl::Block* pBlock = m_pImpl->m_arrFreeBlockHead[index];
		m_pImpl->m_arrFreeBlockHead[index] = pBlock->pNext;

		return pBlock;
	}
	// �����������û�п���λ�ã��򿪱�ָ��������������
	else
	{
		// ���������Ϣ�������ˣ��򿪱�һ�Ÿ������Ϣ��
		if (m_pImpl->m_nChunkCount == m_pImpl->m_nChunkSpace)
		{
			Impl::ChunkInfo* arrOldChunk = m_pImpl->m_arrChunkInfo;

			m_pImpl->m_nChunkSpace += s_nChunkIncrement;
			m_pImpl->m_arrChunkInfo = new Impl::ChunkInfo[m_pImpl->m_nChunkSpace];

			// ת�ƾ�������Ϣ����ʼ����������Ϣ
			memcpy(m_pImpl->m_arrChunkInfo, arrOldChunk, m_pImpl->m_nChunkCount * sizeof(Impl::ChunkInfo));
			memset(m_pImpl->m_arrChunkInfo + m_pImpl->m_nChunkCount, 0, s_nChunkIncrement * sizeof(Impl::ChunkInfo));
			delete[] arrOldChunk;
		}

		// ���������鲢��ʼ��
		Impl::ChunkInfo* pChunkInfo = m_pImpl->m_arrChunkInfo + m_pImpl->m_nChunkCount;
		pChunkInfo->pBlocks = (Impl::Block*)new char[s_nChunkSize];
		pChunkInfo->nChunkSize = s_arrBlockSize[index];

		int nBlockCount = s_nChunkSize / pChunkInfo->nChunkSize;
		for (int i = 0; i < nBlockCount - 1; i++)
		{
			Impl::Block* pBlock = (Impl::Block*)((uintptr_t)pChunkInfo->pBlocks + i * pChunkInfo->nChunkSize);
			Impl::Block* pNext = (Impl::Block*)((uintptr_t)pChunkInfo->pBlocks + (i + 1) * pChunkInfo->nChunkSize);
			pBlock->pNext = pNext;
		}
		Impl::Block* pLastBlock = (Impl::Block*)((uintptr_t)pChunkInfo->pBlocks + (nBlockCount - 1) * pChunkInfo->nChunkSize);
		pLastBlock->pNext = nullptr;
		m_pImpl->m_nChunkCount++;

		return pChunkInfo->pBlocks;
	}
}

void GameBlockAllocator::Free(void* pObject, size_t nSize)
{
	if (nSize == 0)
	{
		return;
	}

	if (nSize > s_nMaxBlockSize)
	{
		delete[] pObject;
		return;
	}

	int index = m_pImpl->m_arrSizeMap[nSize];
	Impl::Block* pBlock = (Impl::Block*)pObject;
	pBlock->pNext = m_pImpl->m_arrFreeBlockHead[index];
	m_pImpl->m_arrFreeBlockHead[index] = pBlock;
}

void GameBlockAllocator::Clear()
{
	for (int i = 0; i < m_pImpl->m_nChunkCount; i++)
	{
		delete[] m_pImpl->m_arrChunkInfo[i].pBlocks;
	}
	m_pImpl->m_nChunkCount = 0;

	memset(m_pImpl->m_arrChunkInfo, 0, m_pImpl->m_nChunkSpace * sizeof(Impl::ChunkInfo));
	memset(m_pImpl->m_arrFreeBlockHead, 0, sizeof(m_pImpl->m_arrFreeBlockHead));
}

GameBlockAllocator::GameBlockAllocator()
{
	m_pImpl = new Impl();
}

GameBlockAllocator::~GameBlockAllocator()
{
	delete m_pImpl;
}