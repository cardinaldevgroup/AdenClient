#include "GameInputModule.h"

#include "GameEventModule.h"

#include <SDL.h>

class GameInput::Impl
{
public:
	SDL_Event			theEvent;
	GameKeyboard::Event eventKeyboard;
};

void GameInput::Update()
{
	while (SDL_PollEvent(&m_pImpl->theEvent))
	{
		switch (m_pImpl->theEvent.type)
		{
		case SDL_QUIT:
			break;

		case SDL_KEYDOWN:
			m_pImpl->eventKeyboard = 
			{
				{ GameEventManager::Type::KEY_DOWN, sizeof(GameKeyboard::Event) },
				(GameKeyboard::Code)m_pImpl->theEvent.key.keysym.sym,
				(GameKeyboard::Mod)m_pImpl->theEvent.key.keysym.mod
			};
			GameKeyboard::GetInstance().PushEvent(m_pImpl->eventKeyboard);
			break;

		case SDL_KEYUP:
			m_pImpl->eventKeyboard =
			{
				{ GameEventManager::Type::KEY_UP, sizeof(GameKeyboard::Event) },
				(GameKeyboard::Code)m_pImpl->theEvent.key.keysym.sym,
				(GameKeyboard::Mod)m_pImpl->theEvent.key.keysym.mod
			};
			GameKeyboard::GetInstance().PushEvent(m_pImpl->eventKeyboard);
			break;
		}
	}
}

GameInput::GameInput()
{
	m_pImpl = new Impl();
}

GameInput::~GameInput()
{
	delete m_pImpl;
}