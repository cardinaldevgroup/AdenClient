#include "GameSpriteModule.h"

#include "GameBlockAllocator.h"

#include <new>

class GameSprite::Impl
{
public:
	GameImage*			m_pImage;

	GamePoint			m_pointPosition;
	GameSize			m_sizeScale;
	int					m_nZOrder;

	GamePoint			m_pointAnchor;
	float				m_fAngle;
	GameImage::Flip		m_emFlip;

public:
	Impl()
	{
		m_pImage = nullptr;
		m_pointPosition = { 0.0f, 0.0f };
		m_nZOrder = 0;
		m_pointAnchor = { 0.5f, 0.5f };
		m_fAngle = 0.0f;
		m_sizeScale = { 1.0f, 1.0f };
		m_emFlip = GameImage::Flip::NONE;
	}
};

GameImage* GameSprite::GetImage()
{
	return m_pImpl->m_pImage;
}

void GameSprite::SetImage(GameImage* const pImage)
{
	m_pImpl->m_pImage = pImage;
}

const GamePoint& GameSprite::GetPosition()
{
	return m_pImpl->m_pointPosition;
}

void GameSprite::SetPosition(const GamePoint& pointPosition)
{
	m_pImpl->m_pointPosition = pointPosition;
}

const GameSize& GameSprite::GetScale()
{
	return m_pImpl->m_sizeScale;
}

void GameSprite::SetScale(const GameSize& sizeScale)
{
	m_pImpl->m_sizeScale = sizeScale;
}

const int& GameSprite::GetZOrder()
{
	return m_pImpl->m_nZOrder;
}

void GameSprite::SetZOrder(const int& nZOrder)
{
	m_pImpl->m_nZOrder = nZOrder;
}

const GamePoint& GameSprite::GetAnchor()
{
	return m_pImpl->m_pointAnchor;
}

void GameSprite::SetAnchor(const GamePoint& pointAnchor)
{
	m_pImpl->m_pointAnchor = pointAnchor;
}

const float& GameSprite::GetAngle()
{
	return m_pImpl->m_fAngle;
}

void GameSprite::SetAngle(const float& fAngle)
{
	m_pImpl->m_fAngle = fAngle;
}

GameImage::Flip GameSprite::GetFlip()
{
	return m_pImpl->m_emFlip;
}

void GameSprite::SetFlip(GameImage::Flip emFlip)
{ 
	m_pImpl->m_emFlip = emFlip;
}

GameSprite::GameSprite()
{
	m_pImpl = new Impl();
}

GameSprite::~GameSprite()
{
	delete m_pImpl;
}

class GameSpriteFactory::Impl
{
public:

public:

};

GameSprite* GameSpriteFactory::CreateSprite(const GameSprite::Def& defSprite)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSprite));
	GameSprite* pSprite = new (pMem) GameSprite();

	pSprite->m_pImpl->m_pImage = defSprite.pImage;
	pSprite->m_pImpl->m_pointPosition = defSprite.pointPosition;
	pSprite->m_pImpl->m_sizeScale = defSprite.sizeScale;
	pSprite->m_pImpl->m_nZOrder = defSprite.nZOrder;
	pSprite->m_pImpl->m_pointAnchor = defSprite.pointAnchor;
	pSprite->m_pImpl->m_fAngle = defSprite.fAngle;
	pSprite->m_pImpl->m_emFlip = defSprite.emFlip;

	return pSprite;
}

void GameSpriteFactory::DestroySprite(GameSprite* const pSprite)
{
	if (!pSprite) return;

	pSprite->~GameSprite();
	GameBlockAllocator::GetInstance().Free(pSprite, sizeof(GameSprite));
}

GameSpriteFactory::GameSpriteFactory()
{
	m_pImpl = new Impl();
}

GameSpriteFactory::~GameSpriteFactory()
{
	delete m_pImpl;
}