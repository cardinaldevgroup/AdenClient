#include "GameEventModule.h"

#include "GameBlockAllocator.h"

#include <new>
#include <set>
#include <vector>

#include <SDL.h>

class GameEventBase::Impl
{
public:
	std::vector<void*>					m_vecEvents;
	std::set<GameEventBase::Listener*>	m_setListeners;
};

GameEventBase::Listener* GameEventBase::Register(std::function<void(void*)> funcCallback, uint16_t nOrder)
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

void GameKeyboard::Notify()
{

}

void GameKeyboard::PushEvent(void* pEvent)
{

}

void GameMouseButton::Notify()
{

}

void GameMouseButton::PushEvent(void* pEvent)
{

}

void GameMouseMotion::Notify()
{

}

void GameMouseMotion::PushEvent(void* pEvent)
{

}

void GameMouseWheel::Notify()
{

}

void GameMouseWheel::PushEvent(void* pEvent)
{

}

void GameCollision::Notify()
{

}

void GameCollision::PushEvent(void* pEvent)
{

}