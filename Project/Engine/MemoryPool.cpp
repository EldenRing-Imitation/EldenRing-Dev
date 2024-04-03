#include "pch.h"
#include "MemoryPool.h"

/*-----------------
	MemoryPool
------------------*/

MemoryPool::MemoryPool(int allocSize) : _allocSize(allocSize)
{
}

MemoryPool::~MemoryPool()
{
	while (_queue.empty() == false)
	{
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	// Pool�� �޸� �ݳ�
	_queue.push(ptr);

	--_allocCount;
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = nullptr;

	// Pool�� ������ �ִ���?
	if (_queue.empty() == false)
	{
		// ������ �ϳ� �����´�
		header = _queue.front();
		_queue.pop();
	}

	// ������ ���� �����
	if (header == nullptr)
	{
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocSize));
	}	

	++_allocCount;

	return header;
}
