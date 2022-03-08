#ifndef _GAME_FILE_IO_H_
#define _GAME_FILE_IO_H_

#include <string>

class GameFileIO
{
public:
	void* Read(std::string strFilePath);
	void  Write(void* pData, std::string strFilePath);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameFileIO();
	GameFileIO(const GameFileIO&) = delete;
	GameFileIO& operator=(const GameFileIO&) = delete;
	static GameFileIO& GetInstance()
	{
		static GameFileIO instance;
		return instance;
	}
private:
	GameFileIO();
};

#endif // !_GAME_IO_H_