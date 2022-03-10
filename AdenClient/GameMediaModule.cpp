#include "GameMediaModule.h"

class GameMediaManager::Impl
{
public:

public:

};



GameMediaManager::GameMediaManager()
{
	m_pImpl = new Impl();
}

GameMediaManager::~GameMediaManager()
{
	delete m_pImpl;
}