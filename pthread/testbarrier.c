#include "../sysHeader.h"

static pthread_barrier_t bar;

void* run(void* arg)
{
	printf("i am thread.%ld:waiting for the bar\n",pthread_self());
	pthread_barrier_wait(&bar);

	sleep(2);

	printf("thread.%ld continue to work\n",pthread_self());
	pthread_exit(arg);
}

int main(void)
{
	pthread_t p1,p2;
	pthread_barrier_init(&bar,NULL,3);
	void* r1,*r2;

	pthread_create(&p1,NULL,run,(void*)1);
	pthread_create(&p2,NULL,run,(void*)2);

	sleep(1);

	pthread_barrier_wait(&bar);
	printf("i am main thread,now start the work\n");

	pthread_join(p1,&r1);
	pthread_join(p2,&r2);

	printf("work end!r1 = %ld,r2 = %ld\n",(long)r1,(long)r2);

	pthread_barrier_destroy(&bar);
	return 0;
}
