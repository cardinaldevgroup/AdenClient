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

void GameFont::SetKerning(int nKerning)
{
	TTF_SetFontKerning(m_pImpl->m_pFont, nKerning);
}

int GameFont::GetOutline()
{
	return TTF_GetFontOutline(m_pImpl->m_pFont);
}

void GameFont::SetOutline(int nOutlineWidth)
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
	GameTexture*	pGameTexture;
	int				nDuration;

public:
	Impl()
	{
		pGameTexture = nullptr;
		nDuration = 0;
	}
};

GameImage::GameImage()
{
	m_pImpl = new Impl();
}

GameImage::GameImage(Def& def)
{
	m_pImpl = new Impl();
	m_pImpl->pGameTexture = def.pGameTexture;
	m_pImpl->nDuration = def.nDuration;
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

GameTexture* GameGraphicManager::LoadTextureFromFile(std::string strFilePath)
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
	else return nullptr;
}

GameFont* GameGraphicManager::LoadFontFromFile(std::string strFilePath, int nFontSize)
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
	else return nullptr;
}

GameImage* GameGraphicManager::CreateStaticImage(GameTexture* pGameTexture)
{
	if (!pGameTexture) return nullptr;

	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameImage));
	GameImage* arrImages = new (pMem) GameImage[1];
	arrImages[0].m_pImpl->pGameTexture = pGameTexture;
	arrImages[0].m_pImpl->nDuration = 0;

	return arrImages;
}

GameImage* GameGraphicManager::CreateDynamicImage(std::initializer_list<GameImage::Def>& ilGameImages)
{
	size_t nListSize = ilGameImages.size();
	if (nListSize == 0) return nullptr;

	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameImage) * nListSize);
	GameImage* arrImages = new (pMem) GameImage[nListSize];

	return arrImages;
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

void GameGraphicManager::DestroryImage(GameImage* pGameImage)
{
	if (!pGameImage) return;

	pGameImage->~GameImage();
	GameBlockAllocator::GetInstance().Free(pGameImage, sizeof(GameImage));
}

GameGraphicManager::GameGraphicManager()
{
	m_pImpl = new Impl();
}

GameGraphicManager::~GameGraphicManager()
{
	delete m_pImpl;
}