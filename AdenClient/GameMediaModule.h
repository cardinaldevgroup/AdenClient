#ifndef _GAME_MEDIA_MODULE_H_
#define _GAME_MEDIA_MODULE_H_

#include "GameFileIO.h"

#include <string>

class GameMusic
{
public:
	enum class Type
	{
		WAV		= 0,
		MP3		= 1,
		OGG		= 2,
		MOD		= 3,
		MID		= 4,
		UNKNOWN = 5
	};

	enum class Fading
	{
		NONE	= 0,
		OUT		= 1,
		IN		= 2
	};

	Type GetType();

private:
	class Impl;
	Impl* m_pImpl;

public:
	GameMusic();
	~GameMusic();

	friend class GameMediaManager;
};

class GameSound
{
private:
	class Impl;
	Impl* m_pImpl;

public:
	GameSound();
	~GameSound();

	friend class GameMediaManager;
};

class GameMediaManager
{
public:
	GameMusic*			LoadMusicFromFile(const std::string& strFilePath);
	GameSound*			LoadSoundFromFile(const std::string& strFilePath);

	void				PlayMusic(GameMusic* pGameMusic, int nLoop);
	void				PlayMusic(GameMusic* pGameMusic, int nLoop, int msFading);
	void				StopMusic();
	void				StopMusic(int msFading);
	void				PauseMusic();
	void				ResumeMusic();
	void				RewindMusic();
	bool				CheckMusicPlaying();
	bool				CheckMusicPaused();
	GameMusic::Fading	GetMusicFadingType();

	void				SetMusicVolume(int nVol);
	int					GetMusicVolume();

	void				PlaySound(GameSound* pGameSound, int nLoop);
	
	void				SetSoundVolume(GameSound* pGameSound, int nVol);
	int					GetSoundVolume(GameSound* pGameSound);

	void				FreeMusic(GameMusic* pGameMusic);
	void				FreeSound(GameSound* pGameSound);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameMediaManager();
	GameMediaManager(const GameMediaManager&) = delete;
	GameMediaManager& operator=(const GameMediaManager&) = delete;
	static GameMediaManager& GetInstance()
	{
		static GameMediaManager instance;
		return instance;
	}

private:
	GameMediaManager();
};

#endif // !_GAME_MEDIA_MODULE_H_