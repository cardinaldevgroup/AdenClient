#ifndef _GAME_TIMER_MODULE_H_
#define _GAME_TIMER_MODULE_H_

#include <functional>

class GameTimer
{
private:
	uint32_t				m_nCountDown;
	std::function<void()>	m_funcCallback;

private:
	GameTimer();
	GameTimer(uint32_t nCountDown, std::function<void()> funcCallback);
	~GameTimer() = default;

	friend class GameTimerManager;
};

class GameTimerManager
{
public:
	void		Update();
	uint32_t	GetCurrentFrame();

	GameTimer*	AddTimer(uint32_t nCountDown, std::function<void()> funcCallback);
	void		RemoveTimer(GameTimer* pTimer);

private:
	class	Impl;
	Impl*	m_pImpl;

public:
	~GameTimerManager();
	GameTimerManager(const GameTimerManager&) = delete;
	GameTimerManager& operator=(const GameTimerManager&) = delete;
	static GameTimerManager& GetInstance()
	{
		static GameTimerManager instance;
		return instance;
	}
private:
	GameTimerManager();
};

#endif // !_GAME_TIME_MODULE_H_