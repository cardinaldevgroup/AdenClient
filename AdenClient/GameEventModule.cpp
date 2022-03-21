#include "GameEventModule.h"

#include "GameBlockAllocator.h"

#include <new>
#include <set>
#include <vector>

#include <SDL.h>

class GameEvent::Impl
{
public:
	std::vector<void*>					m_vecEvents;
	std::set<GameEvent::Listener*>	m_setListeners;
};

void GameEvent::Notify()
{
	for (std::vector<void*>::iterator iterEvent = m_pImpl->m_vecEvents.begin();
		iterEvent != m_pImpl->m_vecEvents.end(); iterEvent++)
	{
		for (std::set<GameEvent::Listener*>::iterator iterListener = m_pImpl->m_setListeners.begin();
			iterListener != m_pImpl->m_setListeners.end(); iterListener++)
		{
			(*iterListener)->funcCallback(*iterEvent);
		}
	}
	m_pImpl->m_vecEvents.clear();
}

GameEvent::Listener* GameEvent::Register(std::function<void(void*)> funcCallback, uint16_t nOrder)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEvent::Listener));

	GameEvent::Listener* pListener = new (pMem) GameEvent::Listener{ funcCallback, nOrder };
	m_pImpl->m_setListeners.insert(pListener);
}

void GameEvent::Unregister(GameEvent::Listener* pListener)
{
	m_pImpl->m_setListeners.erase(pListener);

	GameBlockAllocator::GetInstance().Free(pListener, sizeof(GameEvent::Listener));
}

GameEvent::GameEvent()
{
	m_pImpl = new Impl();
}

GameEvent::~GameEvent()
{
	delete m_pImpl;
}

void GameKeyboard::PushEvent(Event* pEvent)
{
	
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