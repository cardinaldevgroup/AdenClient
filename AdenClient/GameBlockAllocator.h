#ifndef _GAME_BLOCK_ALLOCATOR_H_
#define _GAME_BLOCK_ALLOCATOR_H_

class GameBlockAllocator
{
public:
	/* Ϊָ����С�����������ڴ�
	* @param �����С
	* @return ָ��ÿ����ڴ��ָ��
	*/
	void* Allocate(size_t nSize);

	/* �ͷ�ָ���Ӧ���ڴ�
	* @param 
	* pObject: ����ָ��
	* nSize: �����С
	* @return ��
	*/
	void  Free(void* pObject, size_t nSize);

	/* ��������ڴ��
	*/
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