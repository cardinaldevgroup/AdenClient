#include "GameInputModule.h"

#include "GameEventModule.h"

#include <SDL.h>
#include <unordered_map>

class GameInput::Impl
{
public:
	SDL_Event				theEvent;

	GameKeyboardManager::Event		eventKeyboard;
	GameMouseButtonManager::Event	eventMouseButton;
	GameMouseMotionManager::Event	eventMouseMotion;
	GameMouseWheelManager::Event	eventMouseWheel;
	GameCollisionManager::Event	eventCollision;
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
		case SDL_KEYUP:
			m_pImpl->eventKeyboard = 
			{
				{ (GameEventManager::Type)m_pImpl->theEvent.type, sizeof(GameKeyboardManager::Event) },
				(GameKeyboardManager::Code)m_pImpl->theEvent.key.keysym.sym,
				(GameKeyboardManager::Mod)m_pImpl->theEvent.key.keysym.mod
			};
			GameKeyboardManager::GetInstance().PushEvent(m_pImpl->eventKeyboard);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			m_pImpl->eventMouseButton =
			{
				{ (GameEventManager::Type)m_pImpl->theEvent.type, sizeof(GameMouseButtonManager::Event) },
				m_pImpl->theEvent.button.x, m_pImpl->theEvent.button.y,
				m_pImpl->theEvent.button.clicks
			};
			GameMouseButtonManager::GetInstance().PushEvent(m_pImpl->eventMouseButton);
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