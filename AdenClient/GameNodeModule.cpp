#include "GameNodeModule.h"

#include "GameBlockAllocator.h"

#include <new>

class GameNode::Impl
{
public:
	Point		m_pointPosition;
	Point		m_pointScale;
	int			m_nZOrder;

	Point		m_pointAnchor;
	float		m_fRotation;

	int			m_nTag;

	GameNode*	m_pParent;
	GameNode*	m_pChildHead;
	GameNode*	m_pChildPrev;
	GameNode*	m_pChildNext;

public:
	Impl()
	{
		m_pointPosition = { 0.0f, 0.0f };
		m_pointScale = { 1.0f, 1.0f };
		m_nZOrder = 0;

		m_pointAnchor = { 0.5f, 0.5f };
		m_fRotation = 0.0f;

		m_nTag = 0;

		m_pParent = nullptr;
		m_pChildHead = nullptr;
		m_pChildPrev = nullptr;
		m_pChildNext = nullptr;
	}
};

const GameNode::Point& GameNode::GetPosition()
{
	return m_pImpl->m_pointPosition;
}

void GameNode::SetPosition(const GameNode::Point& pointPosition)
{
	m_pImpl->m_pointPosition = pointPosition;
}

const float& GameNode::GetRotation()
{
	return m_pImpl->m_fRotation;
}

void GameNode::SetRotation(const float& fRotation)
{
	m_pImpl->m_fRotation = fRotation;
}

const GameNode::Point& GameNode::GetScale()
{
	return m_pImpl->m_pointScale;
}

void GameNode::SetScale(const GameNode::Point& pointScale)
{
	m_pImpl->m_pointScale = pointScale;
}

const GameNode::Point& GameNode::GetAnchor()
{
	return m_pImpl->m_pointAnchor;
}

void GameNode::SetAnchor(const GameNode::Point& pointAnchor)
{
	m_pImpl->m_pointAnchor = pointAnchor;
}

const int& GameNode::GetZOrder()
{
	return m_pImpl->m_nZOrder;
}

void GameNode::SetZOrder(const int& nZOrder)
{
	m_pImpl->m_nZOrder = nZOrder;
}

const int& GameNode::GetTag()
{
	return m_pImpl->m_nTag;
}

void GameNode::SetTag(const int& nTag)
{
	m_pImpl->m_nTag = nTag;
}

GameNode* GameNode::GetParent()
{
	return m_pImpl->m_pParent;
}

void GameNode::SetParent(GameNode* pNode)
{
	// ����ýڵ�ԭ�����и��ڵ㣬��ɾ����ϵ
	if (GetParent())
	{
		GetParent()->RemoveChild(this);
	}
	// ��� pNode ��Ϊ nullptr������Ӹýڵ�
	if (pNode)
	{
		pNode->AddChild(this);
	}
}

GameNode* GameNode::GetChildHead()
{
	return m_pImpl->m_pChildHead;
}

GameNode* GameNode::GetChildNext()
{
	return m_pImpl->m_pChildNext;
}

GameNode* GameNode::GetChildPrev()
{
	return m_pImpl->m_pChildPrev;
}

void GameNode::AddChild(GameNode* pNode)
{
	if (!pNode) return;

	// �����ӽڵ����и��ӹ�ϵ�����Ƴ���ι�ϵ
	if (pNode->GetParent())
	{
		pNode->GetParent()->RemoveChild(pNode);
	}

	pNode->m_pImpl->m_pParent = this;
	pNode->m_pImpl->m_pChildNext = m_pImpl->m_pChildHead;
	if (m_pImpl->m_pChildHead)
	{
		m_pImpl->m_pChildHead->m_pImpl->m_pChildPrev = pNode;
	}
	m_pImpl->m_pChildHead = pNode;
}

void GameNode::RemoveChild(GameNode* pNode)
{
	if (!pNode) return;
	// �� pNode ���Ǹýڵ���ӽڵ㣬�򷵻�
	if (pNode->m_pImpl->m_pParent != this) return;

	pNode->m_pImpl->m_pParent = nullptr;

	// ����ӽڵ�ǰ����ڽڵ㣬��˵��������λ������ͷ
	if (pNode->GetChildPrev())
	{
		pNode->m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext = pNode->GetChildNext();
		if (pNode->GetChildNext())
		{
			pNode->m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev = pNode->GetChildPrev();
		}
		return;
	}
	
	// ����ӽڵ�ǰ�治���ڽڵ㣬��˵����λ������ͷ
	if (pNode->GetChildNext())
	{
		pNode->GetChildNext()->m_pImpl->m_pChildPrev = nullptr;
		m_pImpl->m_pChildHead = pNode->GetChildNext();
		return;
	}

	m_pImpl->m_pChildHead = nullptr;
}

GameNode::GameNode()
{
	m_pImpl = new Impl();
}

GameNode::~GameNode()
{
	delete m_pImpl;
}

class GameNodeFactory::Impl
{
public:
	GameNode* CloneChild(GameNode* pNode, GameNode* pParent)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameNode));
		GameNode* pCloneNode = new (pMem) GameNode();

		pCloneNode->SetPosition(pNode->GetPosition());
		pCloneNode->SetRotation(pNode->GetRotation());
		pCloneNode->SetScale(pNode->GetScale());
		pCloneNode->SetAnchor(pNode->GetAnchor());
		pCloneNode->SetZOrder(pNode->GetZOrder());
		pCloneNode->SetTag(pNode->GetTag());
		pCloneNode->SetParent(pParent);

		for (GameNode* pChild = pNode->GetChildHead(); pChild; pChild = pChild->GetChildNext())
		{
			GameNode* pCloneChild = CloneChild(pChild, pCloneNode);
			pCloneNode->AddChild(pCloneChild);
		}
	}
};

GameNode* GameNodeFactory::CreateNode()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameNode));
	GameNode* pNode = new (pMem) GameNode();
	
	return pNode;
}

GameNode* GameNodeFactory::CreateNode(const GameNode::Def& defNode)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameNode));
	GameNode* pNode = new (pMem) GameNode();

	pNode->SetPosition(defNode.pointPosition);
	pNode->SetRotation(defNode.fRotation);
	pNode->SetScale(defNode.pointScale);
	pNode->SetAnchor(defNode.pointAnchor);
	pNode->SetZOrder(defNode.nZOrder);
	pNode->SetTag(defNode.nTag);

	if (defNode.pParent)
	{
		pNode->SetParent(defNode.pParent);
	}

	return pNode;
}

GameNode* GameNodeFactory::CloneNode(GameNode* pNode)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameNode));
	GameNode* pCloneNode = new (pMem) GameNode();

	pCloneNode->SetPosition(pNode->GetPosition());
	pCloneNode->SetRotation(pNode->GetRotation());
	pCloneNode->SetScale(pNode->GetScale());
	pCloneNode->SetAnchor(pNode->GetAnchor());
	pCloneNode->SetZOrder(pNode->GetZOrder());
	pCloneNode->SetTag(pNode->GetTag());
	pCloneNode->SetParent(pNode->GetParent());

	for (GameNode* pChild = pNode->GetChildHead(); pChild; pChild = pChild->GetChildNext())
	{
		GameNode* pCloneChild = m_pImpl->CloneChild(pChild, pCloneNode);
		pCloneNode->AddChild(pCloneChild);
	}
}

void GameNodeFactory::DestroyNode(GameNode* pNode)
{
	if (!pNode) return;

	for (GameNode* pChild = pNode->GetChildHead(); pChild; pChild = pChild->GetChildNext())
	{
		DestroyNode(pChild);
	}

	pNode->~GameNode();
	GameBlockAllocator::GetInstance().Free(pNode, sizeof(GameNode));
}

GameNodeFactory::GameNodeFactory()
{
	m_pImpl = new Impl();
}

GameNodeFactory::~GameNodeFactory()
{
	delete m_pImpl;
}