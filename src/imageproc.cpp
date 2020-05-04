#include "imageproc.h"
#include "counter.h"
#include "parallels.h"
#include "common.h"

#include <unistd.h>

#ifdef OMP_SYSTEM
#include <omp.h>
#endif

ImageProc::ImageProc(const string &fileName, uint16_t threadCount)
:_fileName(fileName)
,_threadCount(threadCount)
{
}

void ImageProc::start()
{
	std::cout << "Start processing image .." << std::endl;
	std::cout << "File: " << _fileName << std::endl;

	if (_threadCount == 0)
	{
#ifndef OMP_SYSTEM
		_threadCount = static_cast<uint16_t>(sysconf(_SC_NPROCESSORS_ONLN));
		std::cout << "Thread count is not present, defaulting to core number: " << _threadCount << std::endl;
#endif
	}
	else
#ifdef OMP_SYSTEM
		omp_set_num_threads(_threadCount);
#else
		std::cout << "Thread count: " << _threadCount << std::endl;
#endif

	if (!isFileExists(_fileName))
	{
		std::cout << "Can't find file: " << _fileName << std::endl;
		return;
	}

	cv::Mat image = cv::imread(_fileName);
	if (image.empty() || image.size().empty())
	{
		std::cout << "Can't load image" << std::endl;
		return;
	}

	Counter counter;
	process(image);
	counter.show();

	//string outFile = filePath.parent_path().string() + "/" + filePath.stem().string() + "-new.jpg";
	string outFile = _fileName + "-new.jpg";
	cv::imwrite(outFile, image);
	std::cout << "All done, written file: " << outFile << std::endl;
}

void ImageProc::process(cv::Mat &image)
{
	processGrayscale(image);
	processGaussianBlur(image);
}

void ImageProc::processGrayscale(cv::Mat &image)
{
	size_t imageSize = image.total();
	size_t bytesSize = image.total() * image.elemSize();

	ByteArray dest(imageSize, 0);
	ByteArray src(image.data, image.data + bytesSize);

#if defined(OMP_SYSTEM)
	grayscale(src, dest, 0, imageSize);
#else
	if (_threadCount > 1)
		Parallels::startB(_threadCount, imageSize, [&](size_t start, size_t end) {
			grayscale(src, dest, start, end);
		});
	else
		grayscale(src, dest, 0, imageSize);
#endif

	cv::Mat img(image.rows, image.cols, CV_8U, dest.data());
	image = img.clone();
}

void ImageProc::processGaussianBlur(cv::Mat &image)
{
	size_t bytesSize = image.total() * image.elemSize();
	ByteArray dest(bytesSize, 0);
	ByteArray src(image.data, image.data + bytesSize);

#if defined(OMP_SYSTEM)
	gaussianBlur(src, dest, image.cols, image.rows, 0, image.rows, 5);
#else
	if (_threadCount > 1)
		Parallels::startB(_threadCount, image.rows, [&](size_t start, size_t end) {
			gaussianBlur(src, dest, image.cols, image.rows, start, end, 5);
		});
	else
		gaussianBlur(src, dest, image.cols, image.rows, 0, image.rows, 5);
#endif

	cv::Mat img(image.rows, image.cols, CV_8U, dest.data());
	image = img.clone();
}

void ImageProc::grayscale(const ByteArray &src, ByteArray &dest, size_t start, size_t end, bool alpha)
{
	int r = 0;
	int g = 0;
	int b = 0;
	int y = 0;

	int channels = 3;
	if (alpha)
		channels = 4;

	size_t index = start * channels;

#ifdef OMP_SYSTEM
	#pragma omp parallel for
#endif

	for (size_t i = start; i < end; ++i)
	{
		b = src[index++];
		g = src[index++];
		r = src[index++];

		// Skip alpha
		if (alpha)
			index++;

		y = (r * 77) + (g * 151) + (b * 28);
		dest[i] = (y >> 8);
	}
}

void ImageProc::gaussianBlur (const ByteArray &src, ByteArray &dest, int w, int h, int start, int end, int r)
{
	auto rs = std::ceil(r * 2.57);

#ifdef OMP_SYSTEM
	#pragma omp parallel for collapse(2)
#endif

	for(int i = start; i < end; i++)
	{
		for(int j = 0; j < w; j++)
		{
			double val = 0;
			double wsum = 0;
			for(int iy = i - rs; iy < i + rs + 1; iy++)
			{
				for(int ix = j - rs; ix < j + rs + 1; ix++)
				{
					int x = std::min(w - 1, std::max(0, ix));
					int y = std::min(h - 1, std::max(0, iy));
					double dsq = (ix - j) * (ix - j) + (iy - i) * (iy - i);
					double wght = std::exp(-dsq / (2 * r * r )) / (M_PI * 2 * r * r);
					val += src[y * w + x] * wght;
					wsum += wght;
				}
			}

			dest[i * w + j] = std::round(val / wsum);
		}
	}
}
