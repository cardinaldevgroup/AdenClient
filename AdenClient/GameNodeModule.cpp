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

	GameNode*	m_pParent;
	GameNode*	m_pChildHead;
	GameNode*	m_pChildNext;

public:
	Impl()
	{
		m_pointPosition = { 0.0f, 0.0f };
		m_pointScale = { 1.0f, 1.0f };
		m_nZOrder = 0;

		m_pointAnchor = { 0.5f, 0.5f };
		m_fAngle = 0.0f;

		m_pParent = nullptr;
		m_pChildHead = nullptr;
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