#include "counter.h"

Counter::Counter()
{
	_point = std::chrono::high_resolution_clock::now();
}

void Counter::show()
{
	int64_t count = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - _point).count();
	std::cout << "Process time: " << count << " ms" << std::endl;
}
