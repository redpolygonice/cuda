#ifndef PARALLELS_H
#define PARALLELS_H

#include "types.h"

#include <atomic>

typedef std::function<void(size_t start, size_t end)> ParallelFunction;

/** Parallel code execution */
class Parallels
{
private:
	static std::atomic<int> refCount;

public:
	Parallels();
	static void startA(uint16_t threadCount, int forCount, const ParallelFunction &func);
	static void startB(uint16_t threadCount, int forCount, const ParallelFunction &func);
};

#endif // PARALLELS_H
