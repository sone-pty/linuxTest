#include "../sysHeader.h"
#include <time.h>

int* te()
{
	static int i = 1;
	return &i;
}

int main(void)
{/*
	int *p = NULL;
	int j = 2;
	p = &j;
	
	printf("%d\n",*p);

	p = te();
	printf("%d\n",*p);
*/
	time_t tmp;
	time(&tmp);
	printf("%s\n",ctime(&tmp));
	return 0;
}
