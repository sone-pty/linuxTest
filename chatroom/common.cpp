#include "common.h"

void sys_error(const char* tip,int exitcode)
{
	perror(tip);
	exit(exitcode);
}
