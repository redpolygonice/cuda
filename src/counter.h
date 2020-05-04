#ifndef COUNTER_H
#define COUNTER_H

#include "types.h"

/** Time counter */
class Counter
{
private:
	std::chrono::high_resolution_clock::time_point _point;

public:
	Counter();
	void show();
	int64_t get() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::high_resolution_clock::now() - _point).count();
	}
};

#endif // COUNTER_H
