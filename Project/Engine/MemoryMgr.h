#pragma once
#include "Allocator.h"
#include "CEntity.h"

using namespace std;

class MemoryPool;

/*-------------
	Memory
---------------*/

class MemoryMgr
{

	enum
	{
		// ~1024까지 32단위, ~2048까지 128단위, ~4096까지 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	MemoryMgr();
	~MemoryMgr();

public:
	void	init();

	void*	Allocate(int size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// O(1) 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};


//template<typename Type, typename... Args>
//Type* xnew(Args&&... args)
//{
//	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
//	new(memory)Type(forward<Args>(args)...); // placement new
//	return memory;
//}

// Entity 또는 그 파생 클래스가 아닌 경우의 xnew
template<typename Type, typename... Args, typename std::enable_if<!std::is_base_of<CEntity, Type>::value, Type>::type* = nullptr>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	new(memory) Type(std::forward<Args>(args)...); // placement new
	return memory;
}

// Entity 또는 그 파생 클래스인 경우의 xnew
template<typename Type, typename... Args, typename std::enable_if<std::is_base_of<CEntity, Type>::value, Type>::type* = nullptr>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	new(memory) Type(std::forward<Args>(args)...); // placement new
	(static_cast<CEntity*>(memory))->SetManagedByMemory(true);
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xrelease(obj);
}