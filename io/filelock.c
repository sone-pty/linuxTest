#include "../sysHeader.h"

int lock_reg(int fd,int cmd,int type,off_t offset,int where,off_t len)
{
	struct flock lock;
	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = where;
	lock.l_len = len;

	return fcntl(fd,cmd,&lock);
}

int lock_test(int fd,int type,off_t offset,int where,off_t len)
{
	struct flock lock;

	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = where;
	lock.l_len = len;

	if(fcntl(fd,F_GETLK,&lock) < 0)
	{
		perror("fcntl");
		exit(-1);
	}

	if(lock.l_type == F_UNLCK)
		return 0;

	return lock.l_pid;
}

int lockfile(int fd)
{
	int err = lock_reg(fd,F_SETLK,F_WRLCK,0,SEEK_SET,0);
	return err;
}
