#include "GameInputModule.h"

#include "GameEventModule.h"

#include <SDL.h>

class GameInput::Impl
{
public:
	SDL_Event theEvent;
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
			GameKeyboard::Event eventKeyboard
			{
				{ GameEventBase::Type::KEY_DOWN, sizeof(GameKeyboard::Event) },
				(GameKeyboard::Code)m_pImpl->theEvent.key.keysym.sym,
				(GameKeyboard::Mod)m_pImpl->theEvent.key.keysym.mod
			};
			GameKeyboard::GetInstance().PushEvent(eventKeyboard);
			break;

		case SDL_KEYUP:
			GameKeyboard::Event eventKeyboard
			{
				{ GameEventBase::Type::KEY_UP, sizeof(GameKeyboard::Event) },
				(GameKeyboard::Code)m_pImpl->theEvent.key.keysym.sym,
				(GameKeyboard::Mod)m_pImpl->theEvent.key.keysym.mod
			};
			GameKeyboard::GetInstance().PushEvent(eventKeyboard);
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