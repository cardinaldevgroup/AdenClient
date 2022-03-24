#include "GameTimerModule.h"

#include "GameBlockAllocator.h"

#include <set>
#include <new>

GameTimer::GameTimer()
{
	m_nCountDown = 0;
}

GameTimer::GameTimer(uint32_t nCountDown, std::function<void()> funcCallback)
{
	m_nCountDown = nCountDown;
	m_funcCallback = funcCallback;
}

class GameTimerManager::Impl
{
public:
	uint32_t				m_nCurrentFrame;
	std::set<GameTimer*>	m_setTimer;
};

void GameTimerManager::Update()
{
	m_pImpl->m_nCurrentFrame++;

	for (std::set<GameTimer*>::iterator iter = m_pImpl->m_setTimer.begin();
		iter != m_pImpl->m_setTimer.end(); iter++)
	{
		if (--(*iter)->m_nCountDown <= 0)
		{
			(*iter)->m_funcCallback();
			RemoveTimer(*iter);
		}
	}
}

uint32_t GameTimerManager::GetCurrentFrame()
{
	return m_pImpl->m_nCurrentFrame;
}

GameTimer* GameTimerManager::AddTimer(uint32_t nCountDown, std::function<void()> funcCallback)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTimer));
	GameTimer* pTimer = new (pMem) GameTimer(nCountDown, funcCallback);
	return pTimer;
}

void GameTimerManager::RemoveTimer(GameTimer* pTimer)
{
	if (m_pImpl->m_setTimer.find(pTimer) != m_pImpl->m_setTimer.end())
	{
		m_pImpl->m_setTimer.erase(pTimer);
		pTimer->~GameTimer();
		GameBlockAllocator::GetInstance().Free(pTimer, sizeof(GameTimer));
	}
}

GameTimerManager::GameTimerManager()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameTimerManager::~GameTimerManager()
{
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}