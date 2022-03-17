#ifndef _GAME_EVENT_MODULE_H_
#define _GAME_EVENT_MODULE_H_

#include <functional>

class GameEventTheme
{
public:

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameEventTheme();
	GameEventTheme(const GameEventTheme&) = delete;
	GameEventTheme& operator=(const GameEventTheme&) = delete;
	static GameEventTheme& GetInstance()
	{
		static GameEventTheme instance;
		return instance;
	}
private:
	GameEventTheme();
};

#endif // !_GAME_EVENT_MODULE_H_