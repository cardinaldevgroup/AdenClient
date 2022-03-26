#include "GameWidgetModule.h"

const std::function<void()> GameWidget::sm_cbEmpty = []() {};

GameWidget::GameWidget()
{
	m_cbOnClick = sm_cbEmpty;
	m_cbOnEnter = sm_cbEmpty;
	m_cbOnLeave = sm_cbEmpty;
	m_cbOnDown = sm_cbEmpty;
	m_cbOnUp = sm_cbEmpty;
	m_cbOnHover = sm_cbEmpty;
	m_cbOnPush = sm_cbEmpty;
}

GameWidget::~GameWidget()
{

}