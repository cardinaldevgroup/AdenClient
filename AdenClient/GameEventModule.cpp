#include "GameEventModule.h"

#include "GameBlockAllocator.h"

#include <new>
#include <set>
#include <vector>

#include <SDL.h>

class GameEventBase::Impl
{
public:
	std::vector<BaseEvent*>		m_vecEvents;
	std::set<Listener*>			m_setListeners;
};

void GameEventBase::Notify()
{
	for (std::vector<BaseEvent*>::iterator iterEvent = m_pImpl->m_vecEvents.begin();
		iterEvent != m_pImpl->m_vecEvents.end(); iterEvent++)
	{
		for (std::set<GameEventBase::Listener*>::iterator iterListener = m_pImpl->m_setListeners.begin();
			iterListener != m_pImpl->m_setListeners.end(); iterListener++)
		{
			(*iterListener)->funcCallback(*iterEvent);
		}
		GameBlockAllocator::GetInstance().Free(*iterEvent, (*iterEvent)->nSize);
	}
	m_pImpl->m_vecEvents.clear();
}

GameEventBase::Listener* GameEventBase::Register(std::function<void(BaseEvent*)> funcCallback, uint16_t nOrder)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEventBase::Listener));

	GameEventBase::Listener* pListener = new (pMem) GameEventBase::Listener{ funcCallback, nOrder };
	m_pImpl->m_setListeners.insert(pListener);
}

void GameEventBase::Unregister(GameEventBase::Listener* pListener)
{
	m_pImpl->m_setListeners.erase(pListener);

	GameBlockAllocator::GetInstance().Free(pListener, sizeof(GameEventBase::Listener));
}

GameEventBase::GameEventBase()
{
	m_pImpl = new Impl();
}

GameEventBase::~GameEventBase()
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