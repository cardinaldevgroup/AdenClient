#include "GameMediaModule.h"

#include "GameBlockAllocator.h"
#include "GameFileIO.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <new>

class GameMusic::Impl
{
public:
	Mix_Music*	m_pMusic;
	GameFile*	m_pFile;

public:
	Impl()
	{
		m_pMusic	= nullptr;
		m_pFile		= nullptr;
	}

	~Impl()
	{
		if (m_pMusic)
		{
			Mix_FreeMusic(m_pMusic);
		}

		if (m_pFile)
		{
			GameFileIO::GetInstance().DestroyFileInHeap(m_pFile);
		}
	}
};

GameMusic::Type GameMusic::GetType()
{
	switch (Mix_GetMusicType(m_pImpl->m_pMusic))
	{
	case MUS_WAV:	return GameMusic::Type::WAV; break;
	case MUS_MP3:	return GameMusic::Type::MP3; break;
	case MUS_OGG:	return GameMusic::Type::OGG; break;
	case MUS_MOD:	return GameMusic::Type::MOD; break;
	case MUS_MID:	return GameMusic::Type::MID; break;
	default:		return GameMusic::Type::UNKNOWN; break;
	}
}

GameMusic::GameMusic()
{
	m_pImpl = new Impl();
}

GameMusic::~GameMusic()
{
	delete m_pImpl;
}

class GameSound::Impl
{
public:
	Mix_Chunk* m_pSound;

public:
	Impl()
	{
		m_pSound = nullptr;
	}

	~Impl()
	{
		if (m_pSound)
		{
			Mix_FreeChunk(m_pSound);
		}
	}
};

GameSound::GameSound()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameSound::~GameSound()
{
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameMediaManager::Impl
{
public:
	Impl()
	{
		Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}

	~Impl()
	{
		Mix_CloseAudio();
		Mix_Quit();
	}

public:

};

GameMediaManager::GameMediaManager()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameMediaManager::~GameMediaManager()
{
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

GameMusic* GameMediaManager::LoadMusicFromFile(const std::string& strFilePath)
{
	GameFile* musicFile = nullptr;
	if (musicFile = GameFileIO::GetInstance().ReadToHeap(strFilePath))
	{
		Mix_Music* pMusic = nullptr;
		if (pMusic = Mix_LoadMUS_RW(SDL_RWFromMem(musicFile->pData, musicFile->nLength), 1))
		{
			void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameMusic));
			GameMusic* pGameMusic = new (pMem) GameMusic();
			pGameMusic->m_pImpl->m_pMusic = pMusic;
			pGameMusic->m_pImpl->m_pFile = musicFile;

			return pGameMusic;
		}

		GameFileIO::GetInstance().DestroyFileInHeap(musicFile);
		return nullptr;
	}
	
	return nullptr;
}

GameSound* GameMediaManager::LoadSoundFromFile(const std::string& strFilePath)
{
	GameFile soundFile = GameFileIO::GetInstance().Read(strFilePath);

	Mix_Chunk* pSound = nullptr;
	if (pSound = Mix_LoadWAV_RW(SDL_RWFromMem(soundFile.pData, soundFile.nLength), 1))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSound));
		GameSound* pGameSound = new (pMem) GameSound();
		pGameSound->m_pImpl->m_pSound = pSound;

		return pGameSound;
	}

	return nullptr;
}

void GameMediaManager::PlayMusic(GameMusic* pGameMusic, int nLoop)
{
	Mix_PlayMusic(pGameMusic->m_pImpl->m_pMusic, nLoop);
}

void GameMediaManager::PlayMusic(GameMusic* pGameMusic, int nLoop, int msFading)
{
	Mix_FadeInMusic(pGameMusic->m_pImpl->m_pMusic, nLoop, msFading);
}

void GameMediaManager::StopMusic()
{
	Mix_HaltMusic();
}

void GameMediaManager::StopMusic(int msFading)
{
	Mix_FadeOutMusic(msFading);
}

void GameMediaManager::PauseMusic()
{
	Mix_PauseMusic();
}

void GameMediaManager::ResumeMusic()
{
	Mix_ResumeMusic();
}

void GameMediaManager::RewindMusic()
{
	Mix_RewindMusic();
}

bool GameMediaManager::CheckMusicPlaying()
{
	return Mix_PlayingMusic();
}

bool GameMediaManager::CheckMusicPaused()
{
	return Mix_PausedMusic();
}

GameMusic::Fading GameMediaManager::GetMusicFadingType()
{
	switch (Mix_FadingMusic())
	{
	case MIX_FADING_OUT:	return GameMusic::Fading::OUT; break;
	case MIX_FADING_IN:		return GameMusic::Fading::IN; break;
	default:				return GameMusic::Fading::NONE; break;
	}
}

void GameMediaManager::SetMusicVolume(int nVol)
{
	Mix_VolumeMusic(nVol);
}

int GameMediaManager::GetMusicVolume()
{
	return Mix_VolumeMusic(-1);
}

void GameMediaManager::PlaySound(GameSound* pGameSound, int nLoop)
{
	Mix_PlayChannel(-1, pGameSound->m_pImpl->m_pSound, nLoop);
}

void GameMediaManager::SetSoundVolume(GameSound* pGameSound, int nVol)
{
	Mix_VolumeChunk(pGameSound->m_pImpl->m_pSound, nVol);
}

int GameMediaManager::GetSoundVolume(GameSound* pGameSound)
{
	return pGameSound->m_pImpl->m_pSound->volume;
}

void GameMediaManager::FreeMusic(GameMusic* pGameMusic)
{
	if (!pGameMusic) return;

	pGameMusic->~GameMusic();
	GameBlockAllocator::GetInstance().Free(pGameMusic, sizeof(GameMusic));
}

void GameMediaManager::FreeSound(GameSound* pGameSound)
{
	if (!pGameSound) return;

	pGameSound->~GameSound();
	GameBlockAllocator::GetInstance().Free(pGameSound, sizeof(GameSound));
}
