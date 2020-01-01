#include "../std.h"
#include "../sysHeader.h"

int main(void)
{
	char* s = "sonepty";
	char news[strlen(s) + 1];
	int i = strlen(s) - 1;

	news[i + 1] = 0; 
	while(i >= 0)
	{
		news[strlen(s) - i - 1] = *(s + i);
		i--;
	}

	cout << news << endl;
	pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&a);
	pthread_mutex_lock(&a);
	cout << "cant here" << endl;
	return 0;
}
