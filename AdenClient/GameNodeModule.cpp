#include "GameNodeModule.h"

#include "GameBlockAllocator.h"

#include <new>
#include <math.h>

class GameNode::Impl
{
public:
	Point		m_pointPosition;
	Point		m_pointScale;
	int			m_nZOrder;

	Point		m_pointAnchor;
	float		m_fAngle;

	Point		m_pointRelativePosition;
	float		m_fRelativeAngle;
	Point		m_pointRelativeScale;

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

		m_pointRelativePosition = { 0.0f, 0.0f };
		m_fRelativeAngle = 0.0f;
		m_pointRelativeScale = { 1.0f, 1.0f };

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

	for (GameNode* pNode = m_pImpl->m_pChildHead; pNode; pNode = m_pImpl->m_pChildNext)
	{
		pNode->SetPosition({
				m_pImpl->m_pointPosition.x + pNode->m_pImpl->m_pointRelativePosition.x,
				m_pImpl->m_pointPosition.y + pNode->m_pImpl->m_pointRelativePosition.y
			});
	}
}

const GameNode::Point& GameNode::GetScale()
{
	return m_pImpl->m_pointScale;
}

void GameNode::SetScale(const GameNode::Point& pointScale)
{
	m_pImpl->m_pointScale = pointScale;

	for (GameNode* pNode = m_pImpl->m_pChildHead; pNode; pNode = m_pImpl->m_pChildNext)
	{
		pNode->SetScale({
				m_pImpl->m_pointScale.x * pNode->m_pImpl->m_pointRelativePosition.x,
				m_pImpl->m_pointScale.y * pNode->m_pImpl->m_pointRelativePosition.y
			});
	}
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

	for (GameNode* pNode = m_pImpl->m_pChildHead; pNode; pNode = m_pImpl->m_pChildNext)
	{
		// 改变父节点角度引起的子节点位置变化
		pNode->m_pImpl->m_pointPosition =
		{
			m_pImpl->m_pointRelativePosition.x * cosf(fAngle) -
			m_pImpl->m_pointRelativePosition.y * sinf(fAngle) +
			m_pImpl->m_pointPosition.x,

			m_pImpl->m_pointRelativePosition.x * sinf(fAngle) +
			m_pImpl->m_pointRelativePosition.y * cosf(fAngle) +
			m_pImpl->m_pointPosition.y
		};

		pNode->SetAngle(m_pImpl->m_fAngle + pNode->m_pImpl->m_fRelativeAngle);
	}
}

const int& GameNode::GetTag()
{
	return m_pImpl->m_nTag;
}

void GameNode::SetTag(const int& nTag)
{
	m_pImpl->m_nTag = nTag;
}

const GameNode::Point& GameNode::GetRelativePosition()
{
	return m_pImpl->m_pointRelativePosition;
}

void GameNode::SetRelativePosition(const Point& pointPosition)
{
	m_pImpl->m_pointRelativePosition = pointPosition;
}

const float& GameNode::GetRelativeAngle()
{
	return m_pImpl->m_fRelativeAngle;
}

void GameNode::SetRelativeAngle(const float& fAngle)
{
	m_pImpl->m_fRelativeAngle = fAngle;
}

const GameNode::Point& GameNode::GetRelativeScale()
{
	return m_pImpl->m_pointRelativeScale;
}

void GameNode::SetRelativeScale(const GameNode::Point& pointScale)
{
	m_pImpl->m_pointRelativeScale = pointScale;
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
	pNode->m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext = pNode->m_pImpl->m_pChildNext;
	pNode->m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev = pNode->m_pImpl->m_pChildPrev;
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
	pNode->m_pImpl->m_pointRelativePosition = defNode.pointRelativePosition;
	pNode->m_pImpl->m_fRelativeAngle = defNode.fRelativeAngle;
	
	if (defNode.pParent)
	{
		defNode.pParent->AddChild(pNode);
	}

	return pNode;
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