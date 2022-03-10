#include "GameFileIO.h"

#include <fstream>

class GameFileIO::Impl
{
public:
	std::ifstream m_finStream;
	std::ofstream m_foutStream;

public:
	~Impl()
	{
		m_finStream.clear();
		m_finStream.close();

		m_foutStream.clear();
		m_foutStream.close();
	}
};

GameFile GameFileIO::Read(const std::string& strFilePath)
{
	m_pImpl->m_finStream.open(strFilePath, std::ios_base::in | std::ios_base::binary);

	if (!m_pImpl->m_finStream.is_open())
	{
		return { nullptr, 0 };
	}

	m_pImpl->m_finStream.seekg(0, m_pImpl->m_finStream.end);
	int nLength = m_pImpl->m_finStream.tellg();
	char* pData = new char[nLength];
	m_pImpl->m_finStream.seekg(0, m_pImpl->m_finStream.beg);
	m_pImpl->m_finStream.read((char*)pData, nLength);

	m_pImpl->m_finStream.clear();
	m_pImpl->m_finStream.close();

	return { pData, nLength };
}

void GameFileIO::Write(const std::string& strFilePath, const GameFile& file)
{
	m_pImpl->m_foutStream.open(strFilePath, std::ios_base::out | std::ios_base::binary);

	if (!m_pImpl->m_foutStream.is_open())
	{
		return;
	}

	m_pImpl->m_foutStream.write((char*)file.pData, file.nLength) << std::flush;

	m_pImpl->m_foutStream.clear();
	m_pImpl->m_foutStream.close();
}

GameFileIO::GameFileIO()
{
	m_pImpl = new Impl();
}

GameFileIO::~GameFileIO()
{
	delete m_pImpl;
}