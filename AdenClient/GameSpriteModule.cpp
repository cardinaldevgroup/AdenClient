#include "GameSpriteModule.h"

#include "GameBlockAllocator.h"

#include <SDL.h>
#include <new>

class GameSprite::Impl
{
public:
	// 挂载的节点
	GameNode*			m_pNode;

	// 渲染信息
	GameImage*			m_pImage;
	Rect				m_rectDst;
	GameImage::Flip		m_emFlip;
	bool				m_bIsShown;

	// 动图播放相关信息
	int					m_nProgress;
	int					m_nPlaySpeed;

public:
	Impl()
	{
		m_pNode = nullptr;
		m_pImage = nullptr;
		m_rectDst = { 0.0f, 0.0f };
		m_emFlip = GameImage::Flip::NONE;
		m_bIsShown = true;

		m_nProgress = 0;
		m_nPlaySpeed = 0;
	}
};

GameNode* GameSprite::GetNode()
{
	return m_pImpl->m_pNode;
}

GameImage* GameSprite::GetImage()
{
	return m_pImpl->m_pImage;
}

void GameSprite::SetImage(GameImage* pImage)
{
	m_pImpl->m_pImage = pImage;
}

const GameSprite::Rect& GameSprite::GetRect()
{
	return m_pImpl->m_rectDst;
}

void GameSprite::SetRect(const GameSprite::Rect& rectDst)
{
	m_pImpl->m_rectDst = rectDst;
}

GameImage::Flip GameSprite::GetFlip()
{
	return m_pImpl->m_emFlip;
}

void GameSprite::SetFlip(GameImage::Flip emFlip)
{
	m_pImpl->m_emFlip = emFlip;
}

bool GameSprite::IsShown()
{
	return m_pImpl->m_bIsShown;
}

void GameSprite::SetShown(bool bIsShown)
{
	m_pImpl->m_bIsShown = bIsShown;
}

int GameSprite::GetProgress()
{
	return m_pImpl->m_nProgress;
}

void GameSprite::SetProgress(int nProgress)
{
	m_pImpl->m_nProgress = nProgress;
}

int GameSprite::GetPlaySpeed()
{
	return m_pImpl->m_nPlaySpeed;
}

void GameSprite::SetPlaySpeed(int nPlaySpeed)
{
	m_pImpl->m_nPlaySpeed = nPlaySpeed;
}

GameSprite::GameSprite()
{
	m_pImpl = new Impl();
}

GameSprite::~GameSprite()
{
	delete m_pImpl;
}

class GameSpriteManager::Impl
{
public:
	// 摄像头相关数据
	GameNode::Point m_pointCameraPosition;

	float m_fZoom;
	float m_fZoomSpeed;
	float m_fZoomDeceleration;

	float m_fZoomUpperLimit;
	float m_fZoomLowerLimit;

	// 计算中使用到的临时变量
	int m_nWindowWidth, m_nWindowHeight;
};

GameSprite* GameSpriteManager::CreateSprite()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSprite));
	GameSprite* pSprite = new (pMem) GameSprite();

	return pSprite;
}

void GameSpriteManager::DestroySprite(GameSprite* pSprite)
{
	if (!pSprite) return;

	pSprite->~GameSprite();
	GameBlockAllocator::GetInstance().Free(pSprite, sizeof(GameSprite));
}

void GameSpriteManager::Show(GameSprite* pSprite)
{
	GameGraphicManager::GetInstance().GetWindowSize(m_pImpl->m_nWindowWidth, m_pImpl->m_nWindowHeight);

	GameGraphicManager::GetInstance().Draw(pSprite->GetImage(),
		((pSprite->GetNode()->GetPosition().x - m_pImpl->m_pointCameraPosition.x) * 100.0f + m_pImpl->m_nWindowWidth / 2) * m_pImpl->m_fZoom,
		(m_pImpl->m_pointCameraPosition.y - ((pSprite->GetNode()->GetPosition().y - m_pImpl->m_pointCameraPosition.y) * 100.0f)) * m_pImpl->m_fZoom,
		pSprite->GetRect().fWidth * m_pImpl->m_fZoom, pSprite->GetRect().fHeight * m_pImpl->m_fZoom,
		360.0f - pSprite->GetNode()->GetRotation() * 180.0f * 3.14159265359f,
		pSprite->GetNode()->GetAnchor().x, pSprite->GetNode()->GetAnchor().y, pSprite->GetFlip(),
		pSprite->m_pImpl->m_nProgress
	);

	if (pSprite->GetProgress() + pSprite->GetPlaySpeed() >= 0)
	{
		pSprite->SetProgress((pSprite->GetProgress() + pSprite->GetPlaySpeed()) % pSprite->GetImage()->GetDurationCount());
	}
	else
	{
		pSprite->SetProgress(pSprite->GetImage()->GetDurationCount() - (pSprite->GetProgress() + pSprite->GetPlaySpeed()));
	}
}

GameSpriteManager::GameSpriteManager()
{
	m_pImpl = new Impl();
}

GameSpriteManager::~GameSpriteManager()
{
	delete m_pImpl;
}