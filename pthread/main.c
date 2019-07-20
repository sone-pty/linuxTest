#include "sysHeader.h"
#define STATSIZE sizeof(struct stat)


typedef struct STAT
{
	int id;
	int count;
	pthread_mutex_t lock;
}st,*pst;

static pst ptr;

pst init_stat()
{
	pst p;
	if((p = malloc(STATSIZE)) != NULL)
	{
		p -> id = 1;
		p -> count = 0;
		
		if(pthread_mutex_init(&p -> lock,NULL) != 0)
		{
			free(p);
			return;
		}
	}

	return p;
}

void hold_stat(pst p)
{
	pthread_mutex_lock(&p -> lock);
	p -> count++;
	printf("current count = %d\n",p -> count);
	pthread_mutex_unlock(&p -> lock);
}

void release_stat(pst p)
{
	pthread_mutex_lock(&p -> lock);
	if(p -> count == 0)
		return;

	if(--p -> count == 0)
	{
		pthread_mutex_destroy(&p -> lock);
		free(p);
		return;
	}
	printf("current count = %d\n",p -> count);
	pthread_mutex_unlock(&p -> lock);
}

void cleanup(void* arg)
{
	printf("clean up for thread\n");
}

void* proc(void* arg)
{
	pthread_t self;
	self = pthread_self();

	printf("thread id:%lu\n",self);
	
	int i;

	for(i = 0;i < 100;i++)
	{
		hold_stat(ptr);
	}

	pthread_cleanup_push(cleanup,NULL);
	pthread_cleanup_pop(1);

	return (void*)1;
}

//make one detached state thread
int makethread(void* (*fn)(void*),void* arg,pthread_t* ppid)
{
	int err;
	pthread_attr_t	attr;

	err = pthread_attr_init(&attr);
	if(err != 0)
		return err;

	err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	if(err != 0)
		return err;

	err = pthread_create(ppid,&attr,fn,arg);
	if(err != 0)
		return err;

	pthread_attr_destroy(&attr);
	return 0;
}

int main(void)
{
	pthread_t tid;
	void* exitcode;
	ptr = init_stat();

	int err = pthread_create(&tid,NULL,proc,NULL);
	sleep(1);
	
	int index = 0;
	while(index++ < 100)
	{
		release_stat(ptr);
	}

	if(err)
	{
		printf("create thread failed\n");
		exit(-1);
	}
	
	pthread_join(tid,&exitcode);

	printf("exitcode:%ld\n",(long)exitcode);

	return 0;
}
