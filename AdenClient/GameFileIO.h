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

	/* ɾ�����ƹ��캯�����ṩ�ƶ����캯��
	��֤���غ���ֵ�����г����Ч��
	 */
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
	/* ��ָ��·���µ��ļ���ȡ���ڴ�����
	* @param 
	* strFilePath: ·��+�ļ���
	* pData: �������ݵ�ָ��
	* nLength: ���ݳ���
	* @return �������������ݺ��䳤�ȵĽṹ��
	*/
	GameFile	Read(const std::string& strFilePath);

	/* ���ڴ��е�����д�뵽ָ���ļ���ȥ
	* @param
	* strFilePath: ·��+�ļ���
	* file: �������������ݺ��䳤�ȵĽṹ��
	*/
	void		Write(const std::string& strFilePath, const GameFile& file);

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