#include "GameGraphicModule.h"

class GameGraphicManager::Impl
{
public:


public:


};

GameGraphicManager::GameGraphicManager()
{
	m_pImpl = new Impl();
}

GameGraphicManager::~GameGraphicManager()
{
	delete m_pImpl;
}