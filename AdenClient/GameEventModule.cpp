#include "GameEventModule.h"

#include <SDL.h>

class GameEventTheme::Impl
{
public:

public:

};

GameEventTheme::GameEventTheme()
{
	m_pImpl = new Impl();
}

GameEventTheme::~GameEventTheme()
{
	delete m_pImpl;
}