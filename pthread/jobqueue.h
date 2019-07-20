#ifndef _JOBQUEUE_H
#define _JOBQUEUE_H

typedef struct _JOB
{
	struct _JOB* jprev;
	struct _JOB* jnext;
	pthread_t tid;
}job,*pjob;

typedef struct _JOBQUEUE
{
	pjob qhead;
	pjob qtail;
	pthread_rwlock_t rwlock;
}jobqueue,*pjobqueue;

int init_queue(pjob q);
int job_insert(pjobqueue que,pjob job);
int job_append(pjobqueue que,pjob job);
int job_remove(pjobqueue que,pjob job);
pjob job_find(pjobqueue que,pthread_t id);

#endif
