#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <memory>
#include <functional>
#include <sys/epoll.h>

namespace sone
{

class eventloop;

//事件处理派生器
class dispatcher{
public:
	typedef std::function<void()> EventCallback;

	dispatcher(int fd, eventloop* l)
		:events(0), revents(0), loop(l), _fd(fd) {}

	void setReadCallback(EventCallback ec) { read_callback = std::move(ec); }
	void setWriteCallback(EventCallback ec) { write_callback = std::move(ec); }
	void setCloseCallback(EventCallback ec) { close_callback = std::move(ec); }
	/*激活、关闭读写事件*/
	void enableReading() { events |= EPOLLIN; update(); }
	void enableWriting() { events |= EPOLLOUT; update(); }
	void disableReading() { events &= ~EPOLLIN; update(); }
	void disableWriting() { events &= ~EPOLLOUT; update(); }
	void enableRDhup() { events |= EPOLLRDHUP; update(); }
	void disableRDhup() { events &= ~EPOLLRDHUP; update(); }
	//返回fd
	int getFd();
	//返回events
	int getEvents();
	//设置revents
	void setRevents(int revents);
	//处理事件
	void process();
	//返回loop对象
	eventloop* getEventloop() { return loop; }
private:
	//更新在复用器中的状态
	void update();
private:
	//事件回调
	EventCallback read_callback;
	EventCallback write_callback;
	EventCallback close_callback;
	//当前fd所关心的events
	int events;
	//epoll返回的触发的事件，根据这个触发回调
	int revents;
	//关联的eventloop
	eventloop* loop;
	//文件描述符
	int _fd;
};
	
}

#endif
