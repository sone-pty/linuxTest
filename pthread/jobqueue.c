#include "../sysHeader.h"
#include "jobqueue.h"

int init_queue(pjob q)
{
	if(q == NULL)
		return -1;
	int err;

	q->qhead = NULL;
	q->qtail = NULL;

	err = pthread_rwlock_init(&q->rwlock,NULL);

	if(err != 0)
		return err;

	return 0;
}

//insert into the head of queue
int job_insert(pjobqueue que,pjob job)
{
	if(que == NULL || job == NULL)
		return -1;
	
	pthread_rwlock_wrlock(&que->rwlock);
	
	job->jprev = NULL;
	job->jnext = que->qhead;

	if(que->qhead == NULL)
		que->qtail = job;
	else
		que->qhead->jprev = job;

	que->qhead = job;

	pthread_rwlock_unlock(&que->rwlock);
	return 0;
}

//insert into the tail of queue
int job_append(pjobqueue que,pjob job)
{
	if(que == NULL || job == NULL)
		return -1;
	
	pthread_rwlock_wrlock(&que->rwlock);

	job->jprev = que->qtail;
	job->jnext = NULL;

	if(que->qtail == NULL)
		que->qhead = job;
	else
		que->qtail->jnext = job;

	que->qtail = job;
	
	pthread_rwlock_unlock(&que->rwlock);
	return 0;
}

//remove the given job from the queue
int job_remove(pjobqueue que,pjob job)
{
	if(que == NULL || job == NULL)
		return -1;
	pthread_rwlock_wrlock(&que->rwlock);

	pjob temp = que->head;
	if(temp == NULL)
		return -1;
	while(temp != NULL && temp != job)
		temp = temp->jnext;
	
	if(temp == NULL)
		return -1;
	
	pjob pre = temp->jprev;
	pre->jnext = temp->jnext;
	temp->jnext->jprev = pre;
	
	pthread_rwlock_wrlock(&que->rwlock);
	return 0;
}

//find the job with tid
pjob job_find(pjobqueue que,pthread_t id)
{
	if(que == NULL || id == NULL)
		return NULL;

	if(pthread_rwlock_rdlock(&que->rwlock) != 0)
		return NULL;

	pjob temp = que->qhead;

	while(temp != NULL)
	{
		if(temp->tid == id)
			return temp;
		else
		{
			temp = temp->jnext;
		}
	}

	pthread_rwlock_unlock(&que->rwlock);
	return NULL;
}
