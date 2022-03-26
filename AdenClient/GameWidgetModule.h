#ifndef _GAME_WIDGET_MODULE_H_
#define _GAME_WIDGET_MODULE_H_

#include "GameNodeModule.h"
#include "GameEventModule.h"

#include <functional>

class GameWidget
{
public:
	enum class Type
	{
		BUTTON		= 0,
		LABEL		= 1,
		IMAGE		= 2,
		SWITCH		= 3,
		SLIDERBAR	= 4,
		TEXTINPUT	= 5,
		POPTIP		= 6,
	};

	virtual const GameNode::Point& GetPosition(int& x, int& y);
	virtual void GetSize(int& w, int& h);

	virtual void SetPosition(const GameNode::Point& pointPos);
	virtual void SetSize(int& w, int& h);

	virtual void SetOnClick(std::function<void()> cbOnClick);
	virtual void SetOnEnter(std::function<void()> cbOnEnter);
	virtual void SetOnLeave(std::function<void()> cbOnLeave);
	virtual void SetOnDown(std::function<void()> cbOnDown);
	virtual void SetOnUp(std::function<void()> cbOnUp);
	virtual void SetOnHover(std::function<void()> cbHover);
	virtual void SetOnPush(std::function<void()> cbOnPush);

	virtual void Update();

protected:
	std::function<void()> m_cbOnClick;
	std::function<void()> m_cbOnEnter;
	std::function<void()> m_cbOnLeave;
	std::function<void()> m_cbOnDown;
	std::function<void()> m_cbOnUp;
	std::function<void()> m_cbOnHover;
	std::function<void()> m_cbOnPush;

protected:
	virtual bool CheckCursorHit(int nCursorX, int nCursorY);

private:
	GameNode*	m_pNode;
	int			m_nWidth, m_nHeight;
	bool		m_bIsDown, m_bIsHover;
	static const std::function<void()> sm_cbEmpty;

private:
	GameWidget();
	~GameWidget();

	// friend class ÈÝÆ÷
};


class GameButton : public GameWidget
{
public:	


private:
	GameButton();
	~GameButton();
};

#endif // !_GAME_WIDGET_MODULE_H_