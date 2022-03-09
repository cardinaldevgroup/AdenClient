#include "GameBlockAllocator.h"

#include <new>
#include <iostream>
#include <Windows.h>

class A
{
public:
	int a;
	int b;
	int c;

public:
	A(int x, int y, int z)
	{
		a = x; b = y; c = z;
	}
};

#undef main
int main(int argc, char* argv[])
{
	void* mem1 = GameBlockAllocator::GetInstance().Allocate(sizeof(A));
	A* test1 = new (mem1) A(1, 1, 4);

	void* mem2 = GameBlockAllocator::GetInstance().Allocate(sizeof(A));
	A* test2 = new (mem2) A(5, 1, 4);

	std::cout << test1->a << " " << test2->a << std::endl;

	system("pause");
	return 0;
}