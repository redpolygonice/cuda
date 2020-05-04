#ifndef CUDAWRAPPER_H
#define CUDAWRAPPER_H

#include "types.h"

//extern void gaussianBlurCuda(uint8_t *src, uint8_t *dest, int w, int h, int r);
void processCuda(const string &fileName);

#endif // CUDAWRAPPER_H
