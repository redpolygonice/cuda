#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include "types.h"

/** Image processor */
class ImageProc
{
private:
	string _fileName;
	uint16_t _threadCount;

private:
	void process(cv::Mat &image);
	void processGaussianBlur(cv::Mat &image);

	void grayscale(const ByteArray &src, ByteArray &dest, size_t start, size_t end, bool alpha = false);
	void gaussianBlur(const ByteArray &scl, ByteArray &tcl, int w, int h, int start, int end, int r);

public:
	ImageProc(const string &fileName, uint16_t threadCount = 0);
	void start();
	void processGrayscale(cv::Mat &image);
};

#endif // IMAGEPROC_H
