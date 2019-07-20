#include <setjmp.h>
#include "../sysHeader.h"

static jmp_buf env;
void f2();

void f1()
{
	if(setjmp(env) != 0)
		printf("经过f2跳转，第二次调用setjump\n");
	else
		f2();
}

void f2()
{
	printf("调用f2，执行longjump\n");
	longjmp(env,1);
}

int main(void)
{
	f1();
	return 0;
}
