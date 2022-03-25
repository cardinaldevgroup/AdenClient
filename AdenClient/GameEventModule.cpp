#include "GameEventModule.h"

#include "GameBlockAllocator.h"

#include <new>
#include <set>
#include <vector>

#include <SDL.h>

class GameEventManager::Impl
{
public:
	std::vector<GameEvent*>		m_vecEvents;
	std::set<Listener*>			m_setListeners;

	Impl()
	{
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	}
	~Impl()
	{
		SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	}
};

void GameEventManager::Notify()
{
	for (std::vector<GameEvent*>::iterator iterEvent = m_pImpl->m_vecEvents.begin();
		iterEvent != m_pImpl->m_vecEvents.end(); iterEvent++)
	{
		for (std::set<GameEventManager::Listener*>::iterator iterListener = m_pImpl->m_setListeners.begin();
			iterListener != m_pImpl->m_setListeners.end(); iterListener++)
		{
			(*iterListener)->funcCallback(*iterEvent);
		}
		GameBlockAllocator::GetInstance().Free(*iterEvent, (*iterEvent)->nSize);
	}
	m_pImpl->m_vecEvents.clear();
}

GameEventManager::Listener* GameEventManager::Register(std::function<void(GameEvent*)> funcCallback, uint16_t nOrder)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEventManager::Listener));

	GameEventManager::Listener* pListener = new (pMem) GameEventManager::Listener{ funcCallback, nOrder };
	m_pImpl->m_setListeners.insert(pListener);

	return pListener;
}

void GameEventManager::Unregister(GameEventManager::Listener* pListener)
{
	if (m_pImpl->m_setListeners.find(pListener) != m_pImpl->m_setListeners.end())
	{
		pListener->~Listener();
		m_pImpl->m_setListeners.erase(pListener);
		GameBlockAllocator::GetInstance().Free(pListener, sizeof(GameEventManager::Listener));
	}
}

GameEventManager::GameEventManager()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameEventManager::~GameEventManager()
{
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

void GameKeyboardEventManager::PushEvent(GameKeyboardEvent* eventKeyboard)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameKeyboardEvent));
	GameKeyboardEvent* pEvent = new (pMem) GameKeyboardEvent(*eventKeyboard);
	m_pImpl->m_vecEvents.push_back(pEvent);
}

void GameMouseButtonEventManager::PushEvent(GameMouseButtonEvent* eventMouseButton)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameMouseButtonEvent));
	GameMouseButtonEvent* pEvent = new (pMem) GameMouseButtonEvent(*eventMouseButton);
	m_pImpl->m_vecEvents.push_back(pEvent);
}

void GameMouseMotionEventManager::PushEvent(GameMouseMotionEvent* eventMouseMotion)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameMouseMotionEvent));
	GameMouseMotionEvent* pEvent = new (pMem) GameMouseMotionEvent(*eventMouseMotion);
	m_pImpl->m_vecEvents.push_back(pEvent);
}

void GameMouseWheelEventManager::PushEvent(GameMouseWheelEvent* eventMouseWheel)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameMouseWheelEvent));
	GameMouseWheelEvent* pEvent = new (pMem) GameMouseWheelEvent(*eventMouseWheel);
	m_pImpl->m_vecEvents.push_back(pEvent);
}

void GameCollisionEventManager::PushEvent(GameCollisionEvent* eventCollision)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameCollisionEvent));
	GameCollisionEvent* pEvent = new (pMem) GameCollisionEvent(*eventCollision);
	m_pImpl->m_vecEvents.push_back(pEvent);
}