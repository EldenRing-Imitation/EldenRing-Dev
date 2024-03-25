#include "pch.h"
#include "MemoryMgr.h"
#include "MemoryPool.h"

/*-------------
	MemoryMgr
---------------*/

MemoryMgr::MemoryMgr()
{
	int size = 0;
	int tableIndex = 0;

	for (size = 32; size <= 1024; size += 32)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}

	for (; size <= 2048; size += 128)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}

	for (; size <= 4096; size += 256)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}
}

MemoryMgr::~MemoryMgr()
{
	for (MemoryPool* pool : _pools)
		delete pool;

	_pools.clear();
}

void MemoryMgr::init()
{
}

void* MemoryMgr::Allocate(int size)
{
	MemoryHeader* header = nullptr;
	const int allocSize = size + sizeof(MemoryHeader);

	if (allocSize > MAX_ALLOC_SIZE)
	{
		// �޸� Ǯ�� �ִ� ũ�⸦ ����� �Ϲ� �Ҵ�
		header = reinterpret_cast<MemoryHeader*>(::malloc(allocSize));
	}
	else
	{
		// �޸� Ǯ���� �����´�
		header = _poolTable[allocSize]->Pop();
	}

	return MemoryHeader::AttachHeader(header, allocSize);
}

void MemoryMgr::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int allocSize = header->allocSize;
	assert(allocSize > 0);

	if (allocSize > MAX_ALLOC_SIZE)
	{
		// �޸� Ǯ�� �ִ� ũ�⸦ ����� �Ϲ� ����
		::free(header);
	}
	else
	{
		// �޸� Ǯ�� �ݳ��Ѵ�
		_poolTable[allocSize]->Push(header);
	}
}
