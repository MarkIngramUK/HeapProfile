#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Allocators.hpp"

using Allocator = Malloc;

struct Allocation
{
	std::vector<void*> Allocs;
	size_t Size;
	long long Time;
};

long long TotalDuration = 0;

std::string AsSize(const size_t size)
{
	std::stringstream ss;

	ss << std::fixed << std::setprecision(1);

	if (size < 1024)
		ss << size << "B";
	else if (size < 1024 * 1024)
		ss << size / 1024 << "KiB";
	else if (size < 1024 * 1024 * 1024)
		ss << size / (1024.0 * 1024.0) << "MiB";
	else
		ss << size / (1024.0 * 1024.0 * 1024.0) << "GiB";

	return ss.str();
}

Allocation Allocate(const size_t size, const long long previousTime = 0)
{
	const size_t Count = 20000;

	std::vector<void*> allocs;
	allocs.resize(Count);

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < allocs.size(); ++i)
	{
		allocs[i] = Allocator::Alloc(size);
	}

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	const long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	if (previousTime)
		std::cout << "Alloc " << allocs.size() << " * " << AsSize(size) << " = " << duration << "ms (" << duration - previousTime << "ms delta)\n";
	else
		std::cout << "Alloc " << allocs.size() << " * " << AsSize(size) << " = " << duration << "ms\n";

	TotalDuration += duration;

	return { std::move(allocs), size, duration };
}

void FreeEvery(Allocation &allocation, const size_t every)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	size_t count = 0;

	for (size_t i = 0; i < allocation.Allocs.size(); i += every)
	{
		if (allocation.Allocs[i])
		{
			Allocator::Free(allocation.Allocs[i], allocation.Size);
			allocation.Allocs[i] = nullptr;
			++count;
		}
	}

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	const long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Freed " << count << " blocks (of size " << AsSize(allocation.Size) << ") = " << duration << "ms\n";

	TotalDuration += duration;
}

void Write(Allocation &allocation, const std::byte fill)
{
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	size_t count = 0;

	for (void *alloc : allocation.Allocs)
	{
		if (alloc)
		{
			memset(alloc, std::to_integer<int>(fill), allocation.Size);
			++count;
		}
	}

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	const long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Wrote " << count << " blocks (of size " << AsSize(allocation.Size) << ") = " << duration << "ms\n";

	TotalDuration += duration;
}

int main(int argc, char *argv[])
{
	std::cout << "Using allocator: " << Allocator::Name() << "\n\n";

	Allocator::Create();

	std::vector<Allocation> allocations =
	{
		Allocate(1),
		Allocate(2),
		Allocate(4),
		Allocate(8),
		Allocate(16),
		Allocate(32),
		Allocate(64),
		Allocate(128),
		Allocate(256),
		Allocate(512),
		Allocate(1 * 1024),
		Allocate(2 * 1024),
		Allocate(4 * 1024),
		Allocate(8 * 1024),		
		Allocate(16 * 1024),

		Allocate(32 * 1024),
		Allocate(64 * 1024),
		Allocate(128 * 1024),
		Allocate(256 * 1024),

		Allocate(384 * 1024),
		Allocate(512 * 1024),
	};

	std::cout << "\n";

	for (Allocation &allocation : allocations)
	{
		FreeEvery(allocation, 2);
	}

	std::cout << "\n";

	allocations.reserve(allocations.size() * 2);

	for (const Allocation &allocation : allocations)
	{
		allocations.push_back(Allocate(allocation.Size, allocation.Time));
	}

	std::cout << "\nWriting to all allocations...\n";

	for (Allocation &allocation : allocations)
	{
		Write(allocation, static_cast<std::byte>(reinterpret_cast<std::ptrdiff_t>(&allocation) & 0xFF));
	}

	std::cout << "\nTotal time: " << std::fixed << std::setprecision(3) << TotalDuration / 1000.0 << "s\n";

	std::cout << "\nCalculating hash of all memory addresses as return value...\n";

	int hash = 0;

	for (const Allocation &allocation : allocations)
	{
		for (const void *mem : allocation.Allocs)
		{
			hash ^= reinterpret_cast<std::ptrdiff_t>(mem);
		}
	}

	Allocator::Destroy();

	return hash;
}
