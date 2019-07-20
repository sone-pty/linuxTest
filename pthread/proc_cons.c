#include "../sysHeader.h"

//存放共享数据
int buf[1024];
//互斥量
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//条件变量
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;
//保存当前生产应放置的下标
int proc_index;
//保存当前消费应取的下标
int cons_index;
//保存数据编号
int ind;

void produce()
{
	pthread_mutex_lock(&lock);
	
	while(buf[proc_index] != -1)
		pthread_cond_wait(&notfull,&lock);
	
	buf[proc_index] = ind;
	printf("生产者生产了%d\n",ind);
	ind++;

	proc_index++;
	if(proc_index == 1024)
		proc_index = 0;
	pthread_cond_signal(&notempty);
	pthread_mutex_unlock(&lock);
}

void consum()
{
	pthread_mutex_lock(&lock);

	while(proc_index == cons_index && buf[cons_index] == -1)
	{
		pthread_cond_wait(&notempty,&lock);
	}

	int data = buf[cons_index];
	buf[cons_index] == -1;
	printf("消费者消费了%d\n",data);

	cons_index++;
	if(cons_index == 1024)
		cons_index = 0;

	pthread_cond_signal(&notfull);
	pthread_mutex_unlock(&lock);
}

void* producer(void* arg)
{
	while(1)
		produce();
	return NULL;
}

void* consumer(void* arg)
{
	while(1)
		consum();
	return NULL;
}

int main(void)
{
	pthread_t c,p;
	
	memset(buf,-1,sizeof(int)*1024);
	pthread_create(&p,NULL,producer,NULL);
	pthread_create(&c,NULL,consumer,NULL);
	struct timespec tp;
	tp.tv_sec = 0;
	tp.tv_nsec = 10000000;
	nanosleep(&tp,NULL);
	return 0;
}
