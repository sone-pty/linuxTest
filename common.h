#ifndef COMMON_H
#define COMMON_H
#define BUFFSIZE 4096

void sys_error(const char* buf,int exitcode);
void pr_mask();
ssize_t readn(int fd,void* buf,size_t n);
ssize_t writeb(int fd,const void* buf,size_t n);

#endif
