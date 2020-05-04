#include "common.h"
#include <unistd.h>
#include <sys/stat.h>

string i2s(int number)
{
	char szNumber[20];
	sprintf(szNumber, "%d", number);
	return szNumber;
}

int s2i(const string &text)
{
	return atoi(text.c_str());
}

bool isFileExists(const std::string &fileName)
{
	struct stat st;
	if (stat(fileName.c_str(), &st) == -1)
		return false;

	return true;
}

string currentTime()
{
	std::time_t time = std::time(NULL);
	char timeStr[50];
	std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H-%M-%S", std::localtime(&time));
	return timeStr;
}
