#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
using std::string;

#include "opencv/cv.hpp"
#include "opencv/highgui.h"

typedef std::vector<uint8_t> ByteArray;
typedef std::vector<uint8_t>::iterator ByteArrayIterator;
typedef std::vector<uint8_t>::const_iterator ByteArrayConstIterator;

//#define OMP_SYSTEM
#define CUDA_SYSTEM

#endif // TYPES_H
