#include <event.h>
#include <stdio.h>

void OnTime(int sock, short event, void *arg)
{
    printf("Game\n");

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    // 重新添加定时事件（定时事件触发后默认自动删除）
    event_add((struct event*)arg, &tv);
}

int main(void)
{
	    // 初始化
    event_init();

    struct event evTimer;
    // 设置定时事件
    evtimer_set(&evTimer, OnTime, &evTimer);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    // 添加定时事件
    event_add(&evTimer, &tv);

    // 事件循环
    event_dispatch();

    return 0;
}
