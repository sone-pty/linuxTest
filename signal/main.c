#include "sysHeader.h"
#include "common.h"

int main(void)
{
	sigset_t nmask;
	sigfillset(&nmask);
	sigprocmask(SIG_SETMASK,&nmask,NULL);
	pr_mask();
	return 0;
}
