#include "GameBlockAllocator.h"

class GameBlockAllocator::Impl
{
public:
	size_t m_arrSizeMap[5] = {
		16, 32, 64, 96, 128
	};

public:
	Impl()
	{

	}
};

void* GameBlockAllocator::Allocate(size_t nSize)
{
	if (nSize == 0)
	{
		return nullptr;
	}
}

void GameBlockAllocator::Free(void* pBlock, size_t nSize)
{

}

void GameBlockAllocator::Clear()
{

}

GameBlockAllocator::GameBlockAllocator()
{
	m_pImpl = new Impl();
}

GameBlockAllocator::~GameBlockAllocator()
{
	delete m_pImpl;
}