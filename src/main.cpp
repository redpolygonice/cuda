#include "cmdparser.h"
#include "imageproc.h"
//#include "gaussian.cu"
#include "cudawrapper.hpp"

int main(int argc, char **argv)
{
	CmdParser cmdParser;
	cmdParser.parse(argc, argv);

#ifdef CUDA_SYSTEM
	processCuda(cmdParser.getImageFile());
#else
	ImageProc imageProc(cmdParser.getImageFile(), cmdParser.getThreadCount());
	imageProc.start();
#endif

	return 0;
}
