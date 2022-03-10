#ifndef _GAME_GRAPHIC_MODULE_H_
#define _GAME_GRAPHIC_MODULE_H_

class GameGraphicManager
{
public:


private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameGraphicManager();
	GameGraphicManager(const GameGraphicManager&) = delete;
	GameGraphicManager& operator=(const GameGraphicManager&) = delete;
	static GameGraphicManager& GetInstance()
	{
		static GameGraphicManager instance;
		return instance;
	}

private:
	GameGraphicManager();
};

#endif // !_GAME_GRAPHIC_MODULE_H_