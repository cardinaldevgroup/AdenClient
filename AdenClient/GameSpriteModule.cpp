#include "GameSpriteModule.h"

#include "GameBlockAllocator.h"

#include <new>

class GameSprite::Impl
{
public:
	GameImage*			m_pImage;
	GameImage::Flip		m_emFlip;

public:
	Impl()
	{
		m_pImage = nullptr;
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
	GameSprite* CloneSpriteChild(GameSprite* pSprite, GameSprite* pParent)
	{

	}
};

GameSprite* GameSpriteFactory::CreateSprite()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSprite));
	GameSprite* pSprite = new (pMem) GameSprite();

	return pSprite;
}

GameSprite* GameSpriteFactory::CreateSprite(const GameSprite::Def& defSprite)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSprite));
	GameSprite* pSprite = new (pMem) GameSprite();

	pSprite->m_pImpl->m_pImage = defSprite.pImage;
	pSprite->m_pImpl->m_emFlip = defSprite.emFlip;
	
	pSprite->SetPosition(defSprite.pointPosition);
	pSprite->SetZOrder(defSprite.nZOrder);
	pSprite->SetScale(defSprite.pointScale);
	pSprite->SetAngle(defSprite.fAngle);
	pSprite->SetAnchor(defSprite.pointAnchor);
	pSprite->SetZOrder(defSprite.nTag);
	pSprite->SetParent(defSprite.pParent);

	return pSprite;
}

GameSprite* GameSpriteFactory::CloneSprite(GameSprite* pSprite)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameNode));
	GameSprite* pCloneSprite = new (pMem) GameSprite();

	pCloneSprite->SetPosition(pSprite->GetPosition());
	pCloneSprite->SetScale(pSprite->GetScale());
	pCloneSprite->SetZOrder(pSprite->GetZOrder());
	pCloneSprite->SetAnchor(pSprite->GetAnchor());
	pCloneSprite->SetAngle(pSprite->GetAngle());
	pCloneSprite->SetTag(pSprite->GetTag());

	pCloneSprite->SetImage(pSprite->GetImage());
	pCloneSprite->SetFlip(pSprite->GetFlip());

	pCloneSprite->SetParent(pSprite->GetParent());

	for (GameNode* pChild = pSprite->GetChildHead(); pSprite; pChild = pChild->GetChildNext())
	{
		GameNode* pCloneChild = m_pImpl->CloneSpriteChild(pChild, pCloneSprite);
		pCloneChild->AddChild(pCloneChild);
	}
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