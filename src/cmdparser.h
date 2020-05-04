#ifndef CMDPARSER_H
#define CMDPARSER_H

#include "types.h"

/** Command line parser */
class CmdParser
{
private:
	string _imageFile;
	uint16_t _threadCount;

public:
	CmdParser();
	void parse(int argc, char **argv);
	inline string getImageFile() const { return _imageFile; }
	inline uint16_t getThreadCount() const { return _threadCount; }
};

#endif // CMDPARSER_H
