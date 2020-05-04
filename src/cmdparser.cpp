#include "cmdparser.h"

CmdParser::CmdParser()
:_threadCount(0)
{
}

void CmdParser::parse(int argc, char **argv)
{
	if (argc >= 2)
		_imageFile = argv[1];

	if (argc == 3)
		_threadCount = static_cast<uint16_t>(std::stoi(argv[2]));
}
