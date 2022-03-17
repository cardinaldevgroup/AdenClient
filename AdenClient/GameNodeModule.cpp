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
	float		m_fAngle;

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
		m_fAngle = 0.0f;

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

const GameNode::Point& GameNode::GetScale()
{
	return m_pImpl->m_pointScale;
}

void GameNode::SetScale(const GameNode::Point& pointScale)
{
	m_pImpl->m_pointScale = pointScale;
}

const int& GameNode::GetZOrder()
{
	return m_pImpl->m_nZOrder;
}

void GameNode::SetZOrder(const int& nZOrder)
{
	m_pImpl->m_nZOrder = nZOrder;
}

const GameNode::Point& GameNode::GetAnchor()
{
	return m_pImpl->m_pointAnchor;
}

void GameNode::SetAnchor(const GameNode::Point& pointAnchor)
{
	m_pImpl->m_pointAnchor = pointAnchor;
}

const float& GameNode::GetAngle()
{
	return m_pImpl->m_fAngle;
}

void GameNode::SetAngle(const float& fAngle)
{
	m_pImpl->m_fAngle = fAngle;
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
	// 若 pNode 为 nullptr，则移除原有父子关系
	if (!pNode)
	{
		if (m_pImpl->m_pParent)
		{
			m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext = m_pImpl->m_pChildNext;
			m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev = m_pImpl->m_pChildPrev;
		}
		m_pImpl->m_pParent = nullptr;
		return;
	}

	// 若 pNode 不为 nullptr，先移除原有父子关系，再添加新父子关系
	if (m_pImpl->m_pParent)
	{
		m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext = m_pImpl->m_pChildNext;
		m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev = m_pImpl->m_pChildPrev;
	}

	m_pImpl->m_pParent = pNode;
	m_pImpl->m_pChildNext = pNode->m_pImpl->m_pChildHead;
	if (pNode->m_pImpl->m_pChildHead)
	{
		pNode->m_pImpl->m_pChildHead->m_pImpl->m_pChildPrev = this;
	}
	pNode->m_pImpl->m_pParent->m_pImpl->m_pChildHead = this;
}

GameNode* GameNode::GetChildHead()
{
	return m_pImpl->m_pChildHead;
}

GameNode* GameNode::GetChildNext()
{
	return m_pImpl->m_pChildNext;
}

void GameNode::AddChild(GameNode* pNode)
{
	if (!pNode) return;

	// 若该子节点已有父子关系，则移除这段父子关系
	if (pNode->m_pImpl->m_pParent)
	{
		pNode->m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext = pNode->m_pImpl->m_pChildNext;
		pNode->m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev = pNode->m_pImpl->m_pChildPrev;
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

	if (pNode->m_pImpl->m_pParent != this) return;

	pNode->m_pImpl->m_pParent = nullptr;

	if (pNode->m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev)
	{
		pNode->m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev = pNode->m_pImpl->m_pChildPrev;
	}
	if (pNode->m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext)
	{
		pNode->m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext = pNode->m_pImpl->m_pChildNext;
	}
}

GameNode::GameNode()
{
	m_pImpl = new Impl();
}

GameNode::~GameNode()
{
	if (m_pImpl->m_pParent)
	{
		m_pImpl->m_pParent->RemoveChild(this);
	}

	for (GameNode* pChild = GetChildHead(); pChild; pChild = pChild->GetChildNext())
	{
		GameNodeFactory::GetInstance().DestroyNode(pChild);
	}

	delete m_pImpl;
}

class GameNodeFactory::Impl
{
public:
	GameNode* CloneNodeChild(GameNode* pNode, GameNode* pParent)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameNode));
		GameNode* pCloneNode = new (pMem) GameNode();

		pCloneNode->m_pImpl->m_pointPosition = pNode->m_pImpl->m_pointPosition;
		pCloneNode->m_pImpl->m_pointScale = pNode->m_pImpl->m_pointScale;
		pCloneNode->m_pImpl->m_nZOrder = pNode->m_pImpl->m_nZOrder;
		pCloneNode->m_pImpl->m_pointAnchor = pNode->m_pImpl->m_pointAnchor;
		pCloneNode->m_pImpl->m_fAngle = pNode->m_pImpl->m_fAngle;
		pCloneNode->m_pImpl->m_nTag = pNode->m_pImpl->m_nTag;

		pCloneNode->m_pImpl->m_pParent = pParent;

		for (GameNode* pChild = pNode->GetChildHead(); pChild; pChild = pChild->GetChildNext())
		{
			GameNode* pCloneChild = CloneNodeChild(pChild, pCloneNode);
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

	pNode->m_pImpl->m_pointPosition = defNode.pointPosition;
	pNode->m_pImpl->m_pointScale = defNode.pointScale;
	pNode->m_pImpl->m_nZOrder = defNode.nZOrder;
	pNode->m_pImpl->m_pointAnchor = defNode.pointAnchor;
	pNode->m_pImpl->m_fAngle = defNode.fAngle;
	pNode->m_pImpl->m_nTag = defNode.nTag;

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

	pCloneNode->m_pImpl->m_pointPosition = pNode->m_pImpl->m_pointPosition;
	pCloneNode->m_pImpl->m_pointScale = pNode->m_pImpl->m_pointScale;
	pCloneNode->m_pImpl->m_nZOrder = pNode->m_pImpl->m_nZOrder;
	pCloneNode->m_pImpl->m_pointAnchor = pNode->m_pImpl->m_pointAnchor;
	pCloneNode->m_pImpl->m_fAngle = pNode->m_pImpl->m_fAngle;
	pCloneNode->m_pImpl->m_nTag = pNode->m_pImpl->m_nTag;

	pCloneNode->m_pImpl->m_pParent = pNode->m_pImpl->m_pParent;

	for (GameNode* pChild = pNode->GetChildHead(); pChild; pChild = pChild->GetChildNext())
	{
		GameNode* pCloneChild = m_pImpl->CloneNodeChild(pChild, pCloneNode);
		pCloneChild->AddChild(pCloneChild);
	}
}

void GameNodeFactory::DestroyNode(GameNode* pNode)
{
	if (!pNode) return;

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