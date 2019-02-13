#pragma once

#include <cstddef>
#include <cstdlib>

#if defined(_WIN32)
#define NOGDI
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

struct Malloc
{
	static const char* Name() { return "malloc/free"; }

	static void Create() {}
	static void Destroy() {}

	static void* Alloc(size_t size)
	{
		return malloc(size);
	}

	static void Free(void *ptr, size_t)
	{
		free(ptr);
	}
};

struct New
{
	static const char* Name() { return "new/delete"; }

	static void Create() {}
	static void Destroy() {}

	static void* Alloc(size_t size)
	{
		return new std::byte[size];
	}

	static void Free(void *ptr, size_t)
	{
		delete[] static_cast<std::byte*>(ptr);
	}
};

#if defined(_WIN32)
struct Heap
{
	static const char* Name() { return "HeapAlloc/HeapFree"; }

	static void Create()
	{
		if (_usePrivateHeap)
		{
			//SYSTEM_INFO systemInfo = {};
			//GetSystemInfo(&systemInfo);

			const size_t committedSize = 0; //1024 * systemInfo.dwPageSize
			_heap = HeapCreate(0, committedSize, 0);
		}
		else
		{
			_heap = GetProcessHeap();
		}
	}

	static void Destroy()
	{
		if (_usePrivateHeap)
			HeapDestroy(_heap);
		_heap = nullptr;
	}

	static void* Alloc(size_t size)
	{
		return HeapAlloc(_heap, 0, size);
	}

	static void Free(void *ptr, size_t)
	{
		HeapFree(_heap, 0, ptr);
	}

private:
	static inline HANDLE _heap = nullptr;
	static inline bool _usePrivateHeap = false;
};

struct Virtual
{
	static const char* Name() { return "VirtualAlloc/VirtualFree"; }

	static void Create() {}
	static void Destroy() {}

	static void* Alloc(size_t size)
	{
		return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}

	static void Free(void *ptr, size_t)
	{
		VirtualFree(ptr, 0, MEM_RELEASE/*MEM_DECOMMIT*/);
	}
};
#endif
