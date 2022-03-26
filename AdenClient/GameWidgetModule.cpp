#include "GameWidgetModule.h"

const std::function<void()> GameWidget::sm_cbEmpty = []() {};

GameWidget::GameWidget()
{
	m_pNode = GameNodeFactory::GetInstance().CreateNode();

	m_nWidth = 0, m_nHeight = 0;

	m_cbOnClick = sm_cbEmpty;
	m_cbOnEnter = sm_cbEmpty;
	m_cbOnLeave = sm_cbEmpty;
	m_cbOnDown = sm_cbEmpty;
	m_cbOnUp = sm_cbEmpty;
	m_cbOnHover = sm_cbEmpty;
	m_cbOnPush = sm_cbEmpty;

	GameMouseMotionEventManager::GetInstance().Register(
		[&](GameEvent* pEvent)
		{
			GameMouseMotionEvent* pMouseMotionEvent = (GameMouseMotionEvent*)pEvent;

			if (CheckCursorHit(pMouseMotionEvent->x, pMouseMotionEvent->y))
			{
				if (!m_bIsHover)
				{
					m_cbOnEnter();
					m_cbOnHover();
					m_bIsHover = true;
				}
			}
			else
			{
				if (m_bIsHover)
				{
					m_cbOnLeave();
					m_bIsHover = false;
				}
			}
		}
	);

	GameMouseButtonEventManager::GetInstance().Register(
		[&](GameEvent* pEvent) 
		{
			GameMouseButtonEvent* pMouseButtonEvent = (GameMouseButtonEvent*)pEvent;

			switch (pMouseButtonEvent->emType)
			{
			case GameEvent::Type::MOUSE_BUTTON_DOWN:
				if (!m_bIsDown && m_bIsHover)
				{
					m_cbOnDown();
					m_cbOnPush();
					m_bIsDown = true;
				}
				break;
			case GameEvent::Type::MOUSE_BUTTON_UP:
				if (m_bIsDown)
				{
					if (m_bIsHover)
					{
						m_cbOnClick();
					}

					m_cbOnUp();
					m_bIsDown = false;
				}
				break;
			default:
				break;
			}
		}
	);
}

bool GameWidget::CheckCursorHit(int nCursorX, int nCursorY)
{
	const GameNode::Point& widgetPos = m_pNode->GetPosition();

	return nCursorX >= widgetPos.x && nCursorX <= widgetPos.x + m_nWidth 
		&& nCursorY >= widgetPos.y && nCursorY <= widgetPos.y + m_nHeight;
}

GameWidget::~GameWidget()
{
	GameNodeFactory::GetInstance().DestroyNode(m_pNode);

	m_pNode = nullptr;
}

void GameWidget::Update()
{
	if (m_bIsHover)
	{
		m_cbOnHover();
	}

	if (m_bIsDown)
	{
		m_cbOnPush();
	}
}

const GameNode::Point& GameWidget::GetPosition(int& x, int& y)
{
	return m_pNode->GetPosition();
}

void GameWidget::GetSize(int& w, int& h)
{
	w = m_nWidth, h = m_nHeight;
}

void GameWidget::SetPosition(const GameNode::Point& pointPos)
{
	m_pNode->SetPosition(pointPos);
}

void GameWidget::SetSize(int& w, int& h)
{
	m_nWidth = w, m_nHeight = h;
}

void GameWidget::SetOnClick(std::function<void()> cbOnClick)
{
	m_cbOnClick = cbOnClick;
}

void GameWidget::SetOnEnter(std::function<void()> cbOnEnter)
{
	m_cbOnEnter = cbOnEnter;
}

void GameWidget::SetOnLeave(std::function<void()> cbOnLeave)
{
	m_cbOnLeave = cbOnLeave;
}

void GameWidget::SetOnDown(std::function<void()> cbOnDown)
{
	m_cbOnDown = cbOnDown;
}

void GameWidget::SetOnUp(std::function<void()> cbOnUp)
{
	m_cbOnUp = cbOnUp;
}

void GameWidget::SetOnHover(std::function<void()> cbHover)
{
	m_cbOnHover = cbHover;
}

void GameWidget::SetOnPush(std::function<void()> cbOnPush)
{
	m_cbOnPush = cbOnPush;
}
