#ifndef _GAME_SPRITE_MODULE_H_
#define _GAME_SPRITE_MODULE_H_

#include "GameGraphicModule.h"

#include "GameNodeModule.h"

class GameSprite: public GameNode
{
public:
	struct Def
	{
		GameImage*			pImage;
		GamePoint			pointPosition;
		GameSize			sizeScale;
		int					nZOrder;
		GamePoint			pointAnchor;
		float				fAngle;
		GameImage::Flip		emFlip;

		Def()
		{
			pImage = nullptr;
			pointPosition = { 0.0f, 0.0f };
			sizeScale = { 1.0f, 1.0f };
			nZOrder = 0;
			pointAnchor = { 0.5f, 0.5f };
			fAngle = 0.0f;
			emFlip = GameImage::Flip::NONE;
		}
	};

	GameImage*			GetImage();
	void				SetImage(GameImage* const pImage);

	const GamePoint&	GetPosition();
	void				SetPosition(const GamePoint& pointPosition);
	
	const GameSize&		GetScale();
	void				SetScale(const GameSize& sizeScale);

	const int&			GetZOrder();
	void				SetZOrder(const int& nZOrder);

	const GamePoint&	GetAnchor();
	void				SetAnchor(const GamePoint& pointAnchor);

	const float&		GetAngle();
	void				SetAngle(const float& fAngle);

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
	GameSprite*		CreateSprite(const GameSprite::Def& defSprite);
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