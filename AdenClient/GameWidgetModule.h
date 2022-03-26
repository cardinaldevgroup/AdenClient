#ifndef _GAME_WIDGET_MODULE_H_
#define _GAME_WIDGET_MODULE_H_

#include "GameNodeModule.h"

#include <functional>

class GameWidget
{
public:
	virtual void GetPosition(int& x, int& y);
	virtual void GetSize(int& w, int& h);

	virtual void SetPosition(int x, int y);
	virtual void SetSize(int& w, int& h);

	virtual void SetOnClick(std::function<void()>);
	virtual void SetOnEnter(std::function<void()>);
	virtual void SetOnLeave(std::function<void()>);
	virtual void SetOnDown(std::function<void()>);
	virtual void SetOnUp(std::function<void()>);
	virtual void SetOnHover(std::function<void()>);
	virtual void SetOnPush(std::function<void()>);

	virtual void Update();

protected:
	std::function<void()> m_cbOnClick;
	std::function<void()> m_cbOnEnter;
	std::function<void()> m_cbOnLeave;
	std::function<void()> m_cbOnDown;
	std::function<void()> m_cbOnUp;
	std::function<void()> m_cbOnHover;
	std::function<void()> m_cbOnPush;

private:
	static const std::function<void()> sm_cbEmpty;
	GameNode* m_pNode;

private:
	GameWidget();
	~GameWidget();

	// friend class ÈÝÆ÷
};


class GameButton : public GameWidget
{
public:
	GameButton();
	~GameButton();

private:

};

#endif // !_GAME_WIDGET_MODULE_H_