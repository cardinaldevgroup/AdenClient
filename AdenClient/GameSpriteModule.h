#ifndef _GAME_SPRITE_MODULE_H_
#define _GAME_SPRITE_MODULE_H_

#include "GameGraphicModule.h"

#include "GameNodeModule.h"

class GameSprite: public GameNode
{
public:
	struct Def : GameNode::Def
	{
		GameImage*			pImage;
		GameImage::Flip		emFlip;

		Def()
		{
			pImage = nullptr;
			emFlip = GameImage::Flip::NONE;
		}
	};

	GameImage*			GetImage();
	void				SetImage(GameImage* const pImage);

	GameImage::Flip		GetFlip();
	void				SetFlip(GameImage::Flip emFlip);

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameSprite();
	~GameSprite();

	friend class GameSpriteFactory;
};

class GameSpriteFactory
{
public:
	GameSprite*		CreateSprite();
	GameSprite*		CreateSprite(const GameSprite::Def& defSprite);
	GameSprite*		CloneSprite(GameSprite* pSprite);
	void			DestroySprite(GameSprite* const pSprite);

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