#include "../sysHeader.h"

int main(void)
{
    pid_t child;

    if((child = fork()) == 0)
    {
        printf("child process gid:%d\n",getgid());
    }

    printf("parent process gid:%d\n",getgid());
    return 0;
}
