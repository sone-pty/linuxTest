#include "../sysHeader.h"

int main(void)
{
	int count = 1;
	alarm(1);

	while(1)
	{
		count++;
		printf("count = %d",count);
	}
	return 0;
}
