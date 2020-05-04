#include "types.h"
#include "common.h"
#include "imageproc.h"
#include "counter.h"
#include "cudawrapper.hpp"

#include <cuda.h>
#include <cuda_runtime.h>

#define CMIN(a,b) (((a)<(b))?(a):(b))
#define CMAX(a,b) (((a)>(b))?(a):(b))

__global__ void gaussianBlurCuda(uchar *src, uchar *dest, int w, int h, int r)
{
	const int col = blockDim.x * blockIdx.x + threadIdx.x;
	const int row = blockDim.y * blockIdx.y + threadIdx.y;

	if (col > w || row > h)
		return;

	auto rs = std::ceil(r * 2.57);

	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			double val = 0;
			double wsum = 0;
			for(int iy = i - rs; iy < i + rs + 1; iy++)
			{
				for(int ix = j - rs; ix < j + rs + 1; ix++)
				{
					int x = CMIN(w - 1 + col, CMAX(0, ix));
					int y = CMIN(h - 1 + row, CMAX(0, iy));
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

void processCuda(const string &fileName)
{
	std::cout << "Start processing image with cuda .." << std::endl;
	std::cout << "File: " << fileName << std::endl;

	if (!isFileExists(fileName))
	{
		std::cout << "Can't find file: " << fileName << std::endl;
		return;
	}

	cv::Mat image = cv::imread(fileName);
	if (image.empty() || image.size().empty())
	{
		std::cout << "Can't load image" << std::endl;
		return;
	}

	Counter counter;
	ImageProc imageProc(fileName);
	imageProc.processGrayscale(image);

	size_t size = image.total();
	ByteArray dest(size, 0);
	ByteArray src(image.data, image.data + size);

	cudaError_t error = cudaSuccess;
	uchar *devDest = NULL;
	uchar *devSrc = NULL;

	int deviceCount = 0;
	error = cudaGetDeviceCount(&deviceCount);
	if (error != cudaSuccess)
	{
		std::cout << "cudaGetDeviceCount error " << error << std::endl;
		return;
	}

	if (deviceCount == 0)
	{
		std::cout << "There is no device!" << std::endl;
		return;
	}

	error = cudaMalloc(&devDest, size);
	if (error != cudaSuccess)
	{
		std::cout << "cudaMalloc error " << error << std::endl;
		return;
	}

	error = cudaMalloc(&devSrc, size);
	if (error != cudaSuccess)
	{
		std::cout << "cudaMalloc error " << error << std::endl;
		return;
	}

	uchar *srcData = src.data();
	error = cudaMemcpy(devSrc, srcData, size, cudaMemcpyHostToDevice);
	if (error != cudaSuccess)
	{
		std::cout << "cudaMemcpy error " << error << std::endl;
		return;
	}

	int threadsPerBlock = 256;
	int blocksPerGrid = (image.total() + threadsPerBlock - 1) / threadsPerBlock;
	gaussianBlurCuda<<<blocksPerGrid, threadsPerBlock>>>(devSrc, devDest, image.cols, image.rows, 15);

	uchar *destData = dest.data();
	cudaMemcpy(destData, devDest, dest.size(), cudaMemcpyDeviceToHost);
	cv::Mat newImage(image.rows, image.cols, CV_8U, destData);

	counter.show();
	string outFile = fileName + "-new.jpg";
	cv::imwrite(outFile, newImage);
	std::cout << "All done, written file: " << outFile << std::endl;

	cudaFree(devSrc);
	cudaFree(devDest);
}
