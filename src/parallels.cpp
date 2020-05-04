#include "parallels.h"

std::atomic<int> Parallels::refCount;

Parallels::Parallels()
{
}

void Parallels::startA(uint16_t threadCount, int forCount, const ParallelFunction &func)
{
	refCount = 0;
	int step = forCount / threadCount;
	int remainder = forCount % threadCount;

	size_t start = 0;
	for (size_t i = 1; i <= threadCount; ++i)
	{
		size_t end = i * step;
		std::thread theThread([start, end, func]() { refCount++; func(start, end); refCount--; });
		theThread.detach();
		start = end;
	}

	if (remainder > 0)
	{
		size_t end = start + remainder;
		std::thread theThread([start, end, func]() { refCount++; func(start, end); refCount--; });
		theThread.detach();
	}

	// Wait all threads finished
	while (refCount > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Parallels::startB(uint16_t threadCount, int forCount, const ParallelFunction &func)
{
	int step = forCount / threadCount;
	int remainder = forCount % threadCount;
	std::vector<std::thread> threads;

	size_t start = 0;
	for (size_t i = 1; i <= threadCount; ++i)
	{
		size_t end = i * step;
		threads.push_back(std::thread([start, end, func]() { func(start, end); }));
		start = end;
	}

	if (remainder > 0)
	{
		size_t end = start + remainder;
		threads.push_back(std::thread([start, end, func]() { func(start, end); }));
	}

	// Wait all threads finished
	for (auto &thread : threads)
		thread.join();
}
