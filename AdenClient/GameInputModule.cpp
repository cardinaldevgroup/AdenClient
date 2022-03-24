#include "GameInputModule.h"

#include "GameEventModule.h"
#include "GameTimerModule.h"
#include "GameBlockAllocator.h"

#include <SDL.h>
#include <unordered_map>

#include <iostream>

class GameInput::Impl
{
public:
	SDL_Event				m_event;

	GameKeyboardEvent		m_eventKeyboard;
	GameMouseButtonEvent	m_eventMouseButton;
	GameMouseMotionEvent	m_eventMouseMotion;
	GameMouseWheelEvent		m_eventMouseWheel;
	GameCollisionEvent		m_eventCollision;

	std::unordered_map<Uint32, std::function<void(void)> > m_mapAnnouncer;

public:
	Impl()
	{
		m_event.type = SDL_FIRSTEVENT;
		m_eventKeyboard = {};
		m_eventMouseButton = {};
		m_eventMouseMotion = {};
		m_eventMouseWheel = {};
		m_eventCollision = {};

		m_mapAnnouncer[SDL_FIRSTEVENT] = nullptr;
		m_mapAnnouncer[SDL_APP_TERMINATING] = nullptr;
		m_mapAnnouncer[SDL_APP_LOWMEMORY] = nullptr;
		m_mapAnnouncer[SDL_APP_WILLENTERBACKGROUND] = nullptr;
		m_mapAnnouncer[SDL_APP_DIDENTERBACKGROUND] = nullptr;
		m_mapAnnouncer[SDL_APP_WILLENTERFOREGROUND] = nullptr;
		m_mapAnnouncer[SDL_APP_DIDENTERFOREGROUND] = nullptr;

		m_mapAnnouncer[SDL_LOCALECHANGED] = nullptr;

		m_mapAnnouncer[SDL_DISPLAYEVENT] = nullptr;

		m_mapAnnouncer[SDL_WINDOWEVENT] = nullptr;
		m_mapAnnouncer[SDL_SYSWMEVENT] = nullptr;

		m_mapAnnouncer[SDL_KEYDOWN] = [&]() {
			m_eventKeyboard =
			{
				{ (GameEvent::Type)m_event.type, GameTimerManager::GetInstance().GetCurrentFrame(),sizeof(GameKeyboardEvent) },
				(GameKeyboardEvent::Code)m_event.key.keysym.sym,
				(GameKeyboardEvent::Mod)m_event.key.keysym.mod
			};
			GameKeyboardEventManager::GetInstance().PushEvent(&m_eventKeyboard);
		};
		m_mapAnnouncer[SDL_KEYUP] = m_mapAnnouncer[SDL_KEYDOWN];
		m_mapAnnouncer[SDL_TEXTEDITING] = nullptr;
		m_mapAnnouncer[SDL_TEXTINPUT] = nullptr;
		m_mapAnnouncer[SDL_KEYMAPCHANGED] = nullptr;

		m_mapAnnouncer[SDL_MOUSEMOTION] = [&]() {
		
		};

		m_mapAnnouncer[SDL_MOUSEBUTTONDOWN] = [&]() {
			m_eventMouseButton =
			{
				{ (GameEvent::Type)m_event.type, GameTimerManager::GetInstance().GetCurrentFrame(),sizeof(GameMouseButtonEvent) },
				m_event.button.x, m_event.button.y,
				m_event.button.clicks
			};
			GameMouseButtonEventManager::GetInstance().PushEvent(&m_eventMouseButton);
		};
		m_mapAnnouncer[SDL_MOUSEBUTTONUP] = m_mapAnnouncer[SDL_MOUSEBUTTONDOWN];

		m_mapAnnouncer[SDL_MOUSEWHEEL] = [&]() {
		
		};

		m_mapAnnouncer[SDL_JOYAXISMOTION] = nullptr;
		m_mapAnnouncer[SDL_JOYBALLMOTION] = nullptr;
		m_mapAnnouncer[SDL_JOYHATMOTION] = nullptr;
		m_mapAnnouncer[SDL_JOYBUTTONDOWN] = nullptr;
		m_mapAnnouncer[SDL_JOYBUTTONUP] = nullptr;
		m_mapAnnouncer[SDL_JOYDEVICEADDED] = nullptr;
		m_mapAnnouncer[SDL_JOYDEVICEREMOVED] = nullptr;

		m_mapAnnouncer[SDL_CONTROLLERAXISMOTION] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERBUTTONDOWN] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERBUTTONUP] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERDEVICEADDED] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERDEVICEREMOVED] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERDEVICEREMAPPED] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERTOUCHPADDOWN] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERTOUCHPADUP] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERTOUCHPADMOTION] = nullptr;
		m_mapAnnouncer[SDL_CONTROLLERSENSORUPDATE] = nullptr;

		m_mapAnnouncer[SDL_FINGERDOWN] = nullptr;
		m_mapAnnouncer[SDL_FINGERUP] = nullptr;
		m_mapAnnouncer[SDL_FINGERMOTION] = nullptr;

		m_mapAnnouncer[SDL_DOLLARGESTURE] = nullptr;
		m_mapAnnouncer[SDL_DOLLARRECORD] = nullptr;
		m_mapAnnouncer[SDL_MULTIGESTURE] = nullptr;

		m_mapAnnouncer[SDL_CLIPBOARDUPDATE] = nullptr;
		
		m_mapAnnouncer[SDL_DROPFILE] = nullptr;
		m_mapAnnouncer[SDL_DROPTEXT] = nullptr;
		m_mapAnnouncer[SDL_DROPBEGIN] = nullptr;
		m_mapAnnouncer[SDL_DROPCOMPLETE] = nullptr;

		m_mapAnnouncer[SDL_AUDIODEVICEADDED] = nullptr;
		m_mapAnnouncer[SDL_AUDIODEVICEREMOVED] = nullptr;

		m_mapAnnouncer[SDL_SENSORUPDATE] = nullptr;

		m_mapAnnouncer[SDL_RENDER_TARGETS_RESET] = nullptr;
		m_mapAnnouncer[SDL_RENDER_DEVICE_RESET] = nullptr;

		m_mapAnnouncer[SDL_USEREVENT] = nullptr;

		m_mapAnnouncer[SDL_LASTEVENT] = nullptr;
	}
};

void GameInput::Update()
{
	while (SDL_PollEvent(&m_pImpl->m_event))
	{
		if (m_pImpl->m_mapAnnouncer[m_pImpl->m_event.type])
		{
			m_pImpl->m_mapAnnouncer[m_pImpl->m_event.type]();
		}
	}
}

GameInput::GameInput()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameInput::~GameInput()
{
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}