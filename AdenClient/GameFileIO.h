#ifndef _GAME_FILE_IO_H_
#define _GAME_FILE_IO_H_

#include <string>

struct GameFile
{
	void*	pData;
	int		nLength;

	GameFile()
	{
		pData = nullptr;
		nLength = 0;
	}
	GameFile(void* data, int length)
	{
		pData = data;
		nLength = length;
	}
	~GameFile()
	{
		delete[] pData;
	}

	// 删除复制构造函数并提供移动构造函数
	// 保证返回函数值过程中程序的效率
	GameFile(GameFile&& move) noexcept
	{
		pData = move.pData;
		nLength = move.nLength;
		move.pData = nullptr;
		move.nLength = 0;
	}
	GameFile(const GameFile& copy) = delete;
};

class GameFileIO
{
public:
	// 将指定路径下的文件读取到内存中来
	// @param strFilePath: 路径+文件名
	// @param pData: 储存数据的指针
	// @param nLength: 数据长度
	// @return 包含二进制数据和其长度的对象
	GameFile	Read(const std::string& strFilePath);

	// 将内存中的数据写入到指定文件中去
	// @param strFilePath: 路径+文件名
	// @param file: 包含二进制数据和其长度的结构体
	void		Write(const std::string& strFilePath, const GameFile& file);

	// 将指定路径下的文件读取到内存中来
	// @param strFilePath: 路径+文件名
	// @param pData: 储存数据的指针
	// @param nLength: 数据长度
	// @return 包含二进制数据和其长度的对象
	GameFile*	ReadToHeap(const std::string& strFilePath);

	// 将读取到堆中的二进制数据释放掉
	// @param pFile: 二进制文件数据指针
	void		DestroyFileInHeap(GameFile* pFile);

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