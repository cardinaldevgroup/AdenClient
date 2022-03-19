#include "GameGraphicModule.h"

#include "GameBlockAllocator.h"
#include "GameFileIO.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <new>

class GameTexture::Impl
{
public:
	int				m_nWidth;
	int				m_nHeight;
	SDL_Texture*	m_pTexture;

public:
	Impl()
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_pTexture = nullptr;
	}
	~Impl()
	{
		if (m_pTexture)
		{
			SDL_DestroyTexture(m_pTexture);
		}
	}
};

int GameTexture::GetWidth()
{
	return m_pImpl->m_nWidth;
}

int GameTexture::GetHeight()
{
	return m_pImpl->m_nHeight;
}

GameTexture::GameTexture()
{
	m_pImpl = new Impl();
}

GameTexture::~GameTexture()
{
	delete m_pImpl;
}

class GameFont::Impl
{
public:
	TTF_Font* m_pFont;

public:
	Impl()
	{
		m_pFont = nullptr;
	}
	~Impl()
	{
		TTF_CloseFont(m_pFont);
	}
};

GameFont::Style GameFont::GetStyle()
{
	return (GameFont::Style)TTF_GetFontStyle(m_pImpl->m_pFont);
}

void GameFont::SetStyle(GameFont::Style emStyle)
{
	TTF_SetFontStyle(m_pImpl->m_pFont, (int)emStyle);
}

GameFont::Hinting GameFont::GetHinting()
{
	return (GameFont::Hinting)TTF_GetFontHinting(m_pImpl->m_pFont);
}

void GameFont::SetHinting(GameFont::Hinting emHinting)
{
	TTF_SetFontHinting(m_pImpl->m_pFont, (int)emHinting);
}

int GameFont::GetKerning()
{
	return TTF_GetFontKerning(m_pImpl->m_pFont);
}

void GameFont::SetKerning(const int& nKerning)
{
	TTF_SetFontKerning(m_pImpl->m_pFont, nKerning);
}

int GameFont::GetOutline()
{
	return TTF_GetFontOutline(m_pImpl->m_pFont);
}

void GameFont::SetOutline(const int& nOutlineWidth)
{
	TTF_SetFontOutline(m_pImpl->m_pFont, nOutlineWidth);
}

GameFont::GameFont()
{
	m_pImpl = new Impl();
}

GameFont::~GameFont()
{
	delete m_pImpl;
}

class GameImage::Impl
{
public:
	Frame*	m_pFrames;
	int		m_nFrameCount;
	int		m_nDurationCount;

public:
	Impl()
	{
		m_pFrames = nullptr;
		m_nFrameCount = 0;
		m_nDurationCount = 0;
	}
	~Impl()
	{
		if (m_pFrames)
		{
			for (int i = 0; i < m_nFrameCount; i++)
			{
				m_pFrames[i].~Frame();
			}
			GameBlockAllocator::GetInstance().Free(m_pFrames, sizeof(Frame) * m_nFrameCount);
		}
	}
};

void GameImage::GetImageSize(int& nWidth, int& nHeight)
{
	nWidth = m_pImpl->m_pFrames[0].pGameTexture->GetWidth();
	nHeight = m_pImpl->m_pFrames[0].pGameTexture->GetHeight();
}

int GameImage::GetFrameCount()
{
	return m_pImpl->m_nFrameCount;
}

int GameImage::GetDurationCount()
{
	return m_pImpl->m_nDurationCount;
}

GameImage::GameImage()
{
	m_pImpl = new Impl();
}

GameImage::~GameImage()
{
	delete m_pImpl;
}

class GameGraphicManager::Impl
{
public:
	SDL_Window*		m_pWindow;
	SDL_Renderer*	m_pRenderer;

	SDL_Rect		m_rectSrc;
	SDL_FRect		m_rectDst;
	SDL_FPoint		m_pointRotation;

public:
	Impl()
	{
		SDL_InitSubSystem(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
		if (!TTF_WasInit()) TTF_Init();

		m_pWindow = SDL_CreateWindow("Aden", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

		SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

		m_rectSrc = { 0, 0, 0, 0 };
		m_rectDst = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pointRotation = { 0.0f, 0.0f };
	}
	~Impl()
	{
		SDL_DestroyWindow(m_pWindow);
		SDL_DestroyRenderer(m_pRenderer);

		TTF_Quit();
		IMG_Quit();
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
};

GameTexture* GameGraphicManager::LoadTextureFromFile(const std::string& strFilePath)
{
	GameFile textureFile = GameFileIO::GetInstance().Read(strFilePath);

	SDL_Surface* pSurface = nullptr;
	if (pSurface = IMG_Load_RW(SDL_RWFromMem(textureFile.pData, textureFile.nLength), 1))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTexture));
		GameTexture* pGameTexture = new (pMem) GameTexture();
		pGameTexture->m_pImpl->m_nWidth = pSurface->w;
		pGameTexture->m_pImpl->m_nHeight = pSurface->h;
		pGameTexture->m_pImpl->m_pTexture = SDL_CreateTextureFromSurface(m_pImpl->m_pRenderer, pSurface);

		SDL_FreeSurface(pSurface);
		return pGameTexture;
	}
	return nullptr;
}

GameFont* GameGraphicManager::LoadFontFromFile(const std::string& strFilePath, const int& nFontSize)
{
	GameFile fontFile = GameFileIO::GetInstance().Read(strFilePath);

	TTF_Font* pFont = nullptr;
	if (pFont = TTF_OpenFontRW(SDL_RWFromMem(fontFile.pData, fontFile.nLength), 1, nFontSize))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameFont));
		GameFont* pGameFont = new (pMem) GameFont();
		pGameFont->m_pImpl->m_pFont = pFont;

		return pGameFont;
	}
	return nullptr;
}

GameImage* GameGraphicManager::CreateImage(GameTexture* pGameTexture)
{
	if (!pGameTexture) return nullptr;

	void* pMemImage = GameBlockAllocator::GetInstance().Allocate(sizeof(GameImage));
	GameImage* pGameImage = new (pMemImage) GameImage();

	void* pMemFrame = GameBlockAllocator::GetInstance().Allocate(sizeof(GameImage::Frame));
	GameImage::Frame* pFrame = new (pMemFrame) GameImage::Frame[1];
	pFrame->pGameTexture = pGameTexture; pFrame->nDuration = 0;

	pGameImage->m_pImpl->m_pFrames = pFrame;
	pGameImage->m_pImpl->m_nFrameCount = 1;
	pGameImage->m_pImpl->m_nDurationCount = 0;

	return pGameImage;
}

GameImage* GameGraphicManager::CreateImage(const std::initializer_list<GameImage::Frame>& ilGameFrames)
{
	int m_nFrameCount = ilGameFrames.size();

	void* pMemImage = GameBlockAllocator::GetInstance().Allocate(sizeof(GameImage));
	GameImage* pGameImage = new (pMemImage) GameImage();
	pGameImage->m_pImpl->m_nFrameCount = m_nFrameCount;

	void* pMemFrames = GameBlockAllocator::GetInstance().Allocate(sizeof(GameImage::Frame) * m_nFrameCount);
	GameImage::Frame* m_pFrames = new (pMemFrames) GameImage::Frame[sizeof(GameImage::Frame) * m_nFrameCount];
	for (int i = 0; i < m_nFrameCount; i++)
	{
		m_pFrames[i].pGameTexture = (ilGameFrames.begin() + i)->pGameTexture;
		m_pFrames[i].nDuration = (ilGameFrames.begin() + i)->nDuration;
		pGameImage->m_pImpl->m_nDurationCount += m_pFrames[i].nDuration;
	}
	pGameImage->m_pImpl->m_pFrames = m_pFrames;

	return pGameImage;
}

void GameGraphicManager::DestroyTexture(GameTexture* pGameTexture)
{
	if (!pGameTexture) return;

	pGameTexture->~GameTexture();
	GameBlockAllocator::GetInstance().Free(pGameTexture, sizeof(GameTexture));
}

void GameGraphicManager::DestroyFont(GameFont* pGameFont)
{
	if (!pGameFont) return;

	pGameFont->~GameFont();
	GameBlockAllocator::GetInstance().Free(pGameFont, sizeof(GameFont));
}

void GameGraphicManager::DestroyImage(GameImage* pGameImage)
{
	if (!pGameImage) return;

	pGameImage->~GameImage();
	GameBlockAllocator::GetInstance().Free(pGameImage, sizeof(GameImage));
}

void GameGraphicManager::Draw(GameImage* pGameImage,
	const float& fDstX, const float& fDstY, const float& fDstW, const float& fDstH,
	const float& fRotation, const float& fAnchorX, const float& fAnchorY, GameImage::Flip emFlip,
	int nProgress)
{
	if (!pGameImage) return;

	int nIndex = 0;
	while (nProgress - pGameImage->m_pImpl->m_pFrames[nIndex].nDuration >= 0)
	{
		nProgress -= pGameImage->m_pImpl->m_pFrames[nIndex].nDuration;
		nIndex = (nIndex + 1) % pGameImage->m_pImpl->m_nFrameCount;
	}

	m_pImpl->m_rectSrc = {
		0, 0,
		pGameImage->m_pImpl->m_pFrames->pGameTexture->GetWidth(),
		pGameImage->m_pImpl->m_pFrames->pGameTexture->GetHeight() 
	};
	m_pImpl->m_rectDst = {
		fDstX - fAnchorX * fDstW,
		fDstY - fAnchorY * fDstH,
		fDstW, fDstH
	};
	m_pImpl->m_pointRotation = { fAnchorX * fDstW, fAnchorY * fDstH };

	SDL_RenderCopyExF(m_pImpl->m_pRenderer, pGameImage->m_pImpl->m_pFrames[nIndex].pGameTexture->m_pImpl->m_pTexture,
		&m_pImpl->m_rectSrc, &m_pImpl->m_rectDst, fRotation, &m_pImpl->m_pointRotation, (SDL_RendererFlip)emFlip);
}

void GameGraphicManager::Draw(GameImage* pGameImage,
	const int& nSrcX, const int& nSrcY, const int& nSrcW, const int& nSrcH,
	const float& fDstX, const float& fDstY, const float& fDstW, const float& fDstH,
	const float& fRotation, const float& fAnchorX, const float& fAnchorY, GameImage::Flip emFlip,
	int nProgress)
{
	if (!pGameImage) return;

	int nIndex = 0;
	while (nProgress - pGameImage->m_pImpl->m_pFrames[nIndex].nDuration >= 0)
	{
		nProgress -= pGameImage->m_pImpl->m_pFrames[nIndex].nDuration;
		nIndex = (nIndex + 1) % pGameImage->m_pImpl->m_nFrameCount;
	}

	m_pImpl->m_rectSrc = { nSrcX, nSrcY, nSrcW, nSrcH };

	m_pImpl->m_rectDst = {
		fDstX - fAnchorX * fDstW,
		fDstY - fAnchorY * fDstH,
		fDstW, fDstH
	};
	m_pImpl->m_pointRotation = { fAnchorX * fDstW, fAnchorY * fDstH };

	SDL_RenderCopyExF(m_pImpl->m_pRenderer, pGameImage->m_pImpl->m_pFrames[nIndex].pGameTexture->m_pImpl->m_pTexture,
		&m_pImpl->m_rectSrc, &m_pImpl->m_rectDst, fRotation, &m_pImpl->m_pointRotation, (SDL_RendererFlip)emFlip);
}

void GameGraphicManager::ClearWindow()
{
	SDL_RenderClear(m_pImpl->m_pRenderer);
}

void GameGraphicManager::PresentWindow()
{
	SDL_RenderPresent(m_pImpl->m_pRenderer);
}

void GameGraphicManager::GetWindowSize(int& nWidth, int& nHeight)
{
	SDL_GetWindowSize(m_pImpl->m_pWindow, &nWidth, &nHeight);
}

void GameGraphicManager::SetWindowSize(int nWidth, int nHeight)
{
	SDL_SetWindowSize(m_pImpl->m_pWindow, nWidth, nHeight);
}

GameGraphicManager::GameGraphicManager()
{
	m_pImpl = new Impl();
}

GameGraphicManager::~GameGraphicManager()
{
	delete m_pImpl;
}