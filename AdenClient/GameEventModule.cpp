#include "GameEventModule.h"

#include "GameBlockAllocator.h"

#include <new>
#include <set>
#include <vector>

#include <SDL.h>

class GameEventManager::Impl
{
public:
	std::vector<BaseEvent*>		m_vecEvents;
	std::set<Listener*>			m_setListeners;
};

void GameEventManager::Notify()
{
	for (std::vector<BaseEvent*>::iterator iterEvent = m_pImpl->m_vecEvents.begin();
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

GameEventManager::Listener* GameEventManager::Register(std::function<void(BaseEvent*)> funcCallback, uint16_t nOrder)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEventManager::Listener));

	GameEventManager::Listener* pListener = new (pMem) GameEventManager::Listener{ funcCallback, nOrder };
	m_pImpl->m_setListeners.insert(pListener);
}

void GameEventManager::Unregister(GameEventManager::Listener* pListener)
{
	m_pImpl->m_setListeners.erase(pListener);

	GameBlockAllocator::GetInstance().Free(pListener, sizeof(GameEventManager::Listener));
}

GameEventManager::GameEventManager()
{
	m_pImpl = new Impl();
}

GameEventManager::~GameEventManager()
{
	delete m_pImpl;
}

void GameKeyboard::PushEvent(const Event& eventKeyboard)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Event));
	Event* pEvent = new (pMem) Event(eventKeyboard);
	m_pImpl->m_vecEvents.push_back(pEvent);
}

void GameMouseButton::PushEvent(Event* pEvent)
{

}

void GameMouseMotion::PushEvent(Event* pEvent)
{

}

void GameMouseWheel::PushEvent(Event* pEvent)
{

}

void GameCollision::PushEvent(Event* pEvent)
{

}