#ifndef _GAME_MEDIA_MODULE_H_
#define _GAME_MEDIA_MODULE_H_

class GameMediaManager
{
public:


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