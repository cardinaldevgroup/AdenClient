#include "GameNodeModule.h"

#include "GameBlockAllocator.h"

#include <new>

class GameNode::Impl
{
public:
	Point		m_pointPosition;
	Point		m_pointScale;
	float		m_fRotation;

	Point		m_pointRelativePosition;
	float		m_fRelativeRotation;
	Point		m_pointRelativeScale;

	Point		m_pointAnchor;
	int			m_nZOrder;

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
		m_fRotation = 0.0f;

		m_pointRelativePosition = { 0.0f, 0.0f };
		m_fRelativeRotation = 0.0f;
		m_pointRelativeScale = { 1.0f, 1.0f };

		m_pointAnchor = { 0.5f, 0.5f };
		m_nZOrder = 0;

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

	for (GameNode* pNode = m_pImpl->m_pChildHead; pNode; pNode = pNode->m_pImpl->m_pChildNext)
	{
		pNode->SetPosition({
				m_pImpl->m_pointPosition.x + pNode->m_pImpl->m_pointRelativePosition.x,
				m_pImpl->m_pointPosition.y + pNode->m_pImpl->m_pointRelativePosition.y
			});
	}
}

const float& GameNode::GetRotation()
{
	return m_pImpl->m_fRotation;
}

void GameNode::SetRotation(const float& fRotation)
{
	m_pImpl->m_fRotation = fRotation;

	for (GameNode* pNode = m_pImpl->m_pChildHead; pNode; pNode = pNode->m_pImpl->m_pChildNext)
	{
		// 改变父节点角度引起的子节点位置变化
		pNode->m_pImpl->m_pointPosition =
		{
			pNode->m_pImpl->m_pointRelativePosition.x * cosf(fRotation) -
			pNode->m_pImpl->m_pointRelativePosition.y * sinf(fRotation) +
			m_pImpl->m_pointPosition.x,

			pNode->m_pImpl->m_pointRelativePosition.x * sinf(fRotation) +
			pNode->m_pImpl->m_pointRelativePosition.y * cosf(fRotation) +
			m_pImpl->m_pointPosition.y
		};

		pNode->SetRotation(m_pImpl->m_fRotation + pNode->m_pImpl->m_fRelativeRotation);
	}
}

const GameNode::Point& GameNode::GetScale()
{
	return m_pImpl->m_pointScale;
}

void GameNode::SetScale(const GameNode::Point& pointScale)
{
	m_pImpl->m_pointScale = pointScale;

	for (GameNode* pNode = m_pImpl->m_pChildHead; pNode; pNode = pNode->m_pImpl->m_pChildNext)
	{
		pNode->SetScale({
				m_pImpl->m_pointScale.x * pNode->m_pImpl->m_pointRelativeScale.x,
				m_pImpl->m_pointScale.y * pNode->m_pImpl->m_pointRelativeScale.y
			});
	}
}

const GameNode::Point& GameNode::GetRelativePosition()
{
	return m_pImpl->m_pointRelativePosition;
}

void GameNode::SetRelativePosition(const Point& pointPosition)
{
	m_pImpl->m_pointRelativePosition = pointPosition;

	if (m_pImpl->m_pParent)
	{
		m_pImpl->m_pointPosition.x = m_pImpl->m_pParent->m_pImpl->m_pointPosition.x + m_pImpl->m_pointRelativePosition.x;
		m_pImpl->m_pointPosition.y = m_pImpl->m_pParent->m_pImpl->m_pointPosition.y + m_pImpl->m_pointRelativePosition.y;
	}
}

const float& GameNode::GetRelativeAngle()
{
	return m_pImpl->m_fRelativeRotation;
}

void GameNode::SetRelativeRotation(const float& fRotation)
{
	m_pImpl->m_fRelativeRotation = fRotation;

	if (m_pImpl->m_pParent)
	{
		m_pImpl->m_fRotation = m_pImpl->m_pParent->m_pImpl->m_fRotation + m_pImpl->m_fRelativeRotation;
	}
}

const GameNode::Point& GameNode::GetRelativeScale()
{
	return m_pImpl->m_pointRelativeScale;
}

void GameNode::SetRelativeScale(const GameNode::Point& pointScale)
{
	m_pImpl->m_pointRelativeScale = pointScale;

	if (m_pImpl->m_pParent)
	{
		m_pImpl->m_pointScale.x = m_pImpl->m_pParent->m_pImpl->m_pointScale.x * m_pImpl->m_pointRelativeScale.x;
		m_pImpl->m_pointScale.y = m_pImpl->m_pParent->m_pImpl->m_pointScale.y * m_pImpl->m_pointRelativeScale.y;
	}
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
	// 如果该节点原来已有父节点，则删除关系
	if (GetParent())
	{
		GetParent()->RemoveChild(this);
	}
	// 如果 pNode 不为 nullptr，则添加该节点
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

	// 若该子节点已有父子关系，则移除这段关系
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
	// 若 pNode 不是该节点的子节点，则返回
	if (pNode->m_pImpl->m_pParent != this) return;

	pNode->m_pImpl->m_pParent = nullptr;

	// 如果子节点前面存在节点，则说明它并不位于链表头
	if (pNode->GetChildPrev())
	{
		pNode->m_pImpl->m_pChildPrev->m_pImpl->m_pChildNext = pNode->GetChildNext();
		if (pNode->GetChildNext())
		{
			pNode->m_pImpl->m_pChildNext->m_pImpl->m_pChildPrev = pNode->GetChildPrev();
		}
		return;
	}
	
	// 如果子节点前面不存在节点，则说明它位于链表头
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