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
		m_nPlaySpeed = -1;
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

	int nWidth = 0, nHeight = 0;
	pImage->GetImageSize(nWidth, nHeight);
	m_pImpl->m_rectDst.fWidth = (float)nWidth;
	m_pImpl->m_rectDst.fHeight = (float)nHeight;
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
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameSprite::~GameSprite()
{
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameSpriteManager::Impl
{
public:
	// 摄像头相关数据
	GameNode::Point	m_pointCameraPosition;

	float			m_fZoom;
	float			m_fZoomSpeed;
	float			m_fZoomDeceleration;

	float			m_fZoomUpperLimit;
	float			m_fZoomLowerLimit;

	// 计算中使用到的临时变量
	int				m_nWindowWidth;
	int				m_nWindowHeight;

public:
	Impl()
	{
		m_pointCameraPosition = { 0.0f, 0.0f };
		m_fZoom = 1.0f;
		m_fZoomSpeed = 0.0f;
		m_fZoomDeceleration = 0.0f;
		m_fZoomUpperLimit = 10.0f;
		m_fZoomLowerLimit = 0.1f;

		GameGraphicManager::GetInstance().GetWindowSize(m_nWindowWidth, m_nWindowHeight);
	}
};

GameSprite* GameSpriteManager::CreateSprite(GameNode* pNode, GameImage* pImage)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSprite));
	GameSprite* pSprite = new (pMem) GameSprite();

	pSprite->m_pImpl->m_pNode = pNode;
	pSprite->SetImage(pImage);

	int nWidth = 0, nHeight = 0;
	pImage->GetImageSize(nWidth, nHeight);
	pSprite->SetRect({ (float)nWidth, (float)nHeight });

	return pSprite;
}

GameSprite* GameSpriteManager::CreateSprite(const GameSprite::Def& defSprite)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSprite));
	GameSprite* pSprite = new (pMem) GameSprite();

	pSprite->m_pImpl->m_pNode = defSprite.pNode;

	pSprite->SetImage(defSprite.pImage);
	pSprite->SetRect(defSprite.rectDst);
	pSprite->SetFlip(defSprite.emFlip);
	pSprite->SetShown(defSprite.bIsShown);
	pSprite->SetProgress(defSprite.nProgress);
	pSprite->SetPlaySpeed(defSprite.nPlaySpeed);

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

		((pSprite->GetNode()->GetPosition().x * pSprite->GetNode()->GetScale().x - m_pImpl->m_pointCameraPosition.x)
			* 100.0f + m_pImpl->m_nWindowWidth / 2) * m_pImpl->m_fZoom,
		(m_pImpl->m_nWindowHeight / 2 - 
			((pSprite->GetNode()->GetPosition().y * pSprite->GetNode()->GetScale().y - m_pImpl->m_pointCameraPosition.y) * 100.0f)) * m_pImpl->m_fZoom,

		pSprite->GetRect().fWidth * pSprite->GetNode()->GetScale().x * m_pImpl->m_fZoom,
		pSprite->GetRect().fHeight * pSprite->GetNode()->GetScale().y * m_pImpl->m_fZoom,

		360.0f - pSprite->GetNode()->GetRotation() * 180.0f / 3.14159265359f,
		pSprite->GetNode()->GetAnchor().x, pSprite->GetNode()->GetAnchor().y, pSprite->GetFlip(),

		pSprite->m_pImpl->m_nProgress
	);

	if (pSprite->GetProgress() + pSprite->GetPlaySpeed() >= 0)
	{
		pSprite->SetProgress((pSprite->GetProgress() + pSprite->GetPlaySpeed()) % pSprite->GetImage()->GetDurationCount());
	}
	else
	{
		pSprite->SetProgress(pSprite->GetImage()->GetDurationCount() + pSprite->GetProgress() + pSprite->GetPlaySpeed());
	}
}

GameSpriteManager::GameSpriteManager()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameSpriteManager::~GameSpriteManager()
{
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}