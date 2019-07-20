#include "IOHeader.h"

DIR* openDir(const char* path)
{
	DIR* ptr = 0;
	if((ptr = opendir(path)) == -1)
	{
		perror("error:");
		return 0;
	}
	return ptr;
}

int int main(int argc, char const *argv[])
{
	return 0;
}