#ifndef _GAME_SPRITE_MODULE_H_
#define _GAME_SPRITE_MODULE_H_

#include "GameGraphicModule.h"
#include "GameNodeModule.h"

class GameSprite
{
public:
	struct Rect
	{
		float fWidth;
		float fHeight;
	};

	struct Def
	{
		GameNode*			pNode;
		GameImage*			pImage;
		Rect				rectDst;
		GameImage::Flip		emFlip;
		bool				bIsShown;

		int					nProgress;
		int					nPlaySpeed;

		Def()
		{
			pNode = nullptr;
			pImage = nullptr;
			rectDst = { 0.0f, 0.0f };
			emFlip = GameImage::Flip::NONE;
			bIsShown = true;
			nProgress = 0;
			nPlaySpeed = 1;
		}
	};

	GameNode*			GetNode();

	GameImage*			GetImage();
	void				SetImage(GameImage* pImage);

	const Rect&			GetRect();
	void				SetRect(const Rect& rectDst);

	GameImage::Flip		GetFlip();
	void				SetFlip(GameImage::Flip emFlip);

	bool				IsShown();
	void				SetShown(bool bIsShown);

	int					GetProgress();
	void				SetProgress(int nProgress);

	int					GetPlaySpeed();
	void				SetPlaySpeed(int nPlaySpeed);

private:
	class	Impl;
	Impl*	m_pImpl;

private:
	GameSprite();
	~GameSprite();

	friend class GameSpriteManager;
};

class GameSpriteManager
{
public:
	GameSprite*		CreateSprite(GameNode* pNode, GameImage* pImage);
	GameSprite*		CreateSprite(const GameSprite::Def& defSprite);
	void			DestroySprite(GameSprite* pSprite);

	void			Show(GameSprite* pSprite);

private:
	class	Impl;
	Impl*	m_pImpl;

public:
	~GameSpriteManager();
	GameSpriteManager(const GameSpriteManager&) = delete;
	GameSpriteManager& operator=(const GameSpriteManager&) = delete;
	static GameSpriteManager& GetInstance()
	{
		static GameSpriteManager instance;
		return instance;
	}
private:
	GameSpriteManager();
};

#endif // !_GAME_SPRITE_MODULE_H_