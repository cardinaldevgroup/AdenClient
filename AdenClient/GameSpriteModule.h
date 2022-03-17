#ifndef _GAME_SPRITE_MODULE_H_
#define _GAME_SPRITE_MODULE_H_

#include "GameGraphicModule.h"

#include "GameNodeModule.h"

class GameSpriteFactory
{
public:


private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameSpriteFactory();
	GameSpriteFactory(const GameSpriteFactory&) = delete;
	GameSpriteFactory& operator=(const GameSpriteFactory&) = delete;
	static GameSpriteFactory& GetInstance()
	{
		static GameSpriteFactory instance;
		return instance;
	}
private:
	GameSpriteFactory();
};

#endif // !_GAME_SPRITE_MODULE_H_