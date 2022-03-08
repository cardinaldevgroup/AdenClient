#ifndef _GAME_BLOCK_ALLOCATOR_H_
#define _GAME_BLOCK_ALLOCATOR_H_

class GameBlockAllocator
{
public:
	void* Allocate(size_t nSize);
	void  Free(void* pBlock, size_t nSize);
	void  Clear();

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameBlockAllocator();
	GameBlockAllocator(const GameBlockAllocator&) = delete;
	GameBlockAllocator& operator=(const GameBlockAllocator&) = delete;
	static GameBlockAllocator& GetInstance()
	{
		static GameBlockAllocator instance;
		return instance;
	}

private:
	GameBlockAllocator();
};

#endif // !_GAME_BLOCK_ALLOCATOR_H_