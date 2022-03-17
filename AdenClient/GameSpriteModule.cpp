#include "GameSpriteModule.h"

#include "GameBlockAllocator.h"

#include <new>


class GameSpriteFactory::Impl
{
public:

};

GameSpriteFactory::GameSpriteFactory()
{
	m_pImpl = new Impl();
}

GameSpriteFactory::~GameSpriteFactory()
{
	delete m_pImpl;
}