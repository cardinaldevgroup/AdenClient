#include "GameFileIO.h"

class GameFileIO::Impl
{
public:


public:


};

void* GameFileIO::Read(std::string strFilePath)
{

}

void GameFileIO::Write(void* pData, std::string strFilePath)
{

}

GameFileIO::GameFileIO()
{
	m_pImpl = new Impl();
}

GameFileIO::~GameFileIO()
{
	delete m_pImpl;
}