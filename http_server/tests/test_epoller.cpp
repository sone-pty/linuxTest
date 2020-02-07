#include "test.h"
#include "../epoller.h"
#include "../dispatcher.h"
#include "../Socket.h"
#include "../eventloop.h"


void closecall(void* p)
{
	eventloop* ptr = static_cast<eventloop*>(p);
	ptr->exit();
}

void readcall(void* p, void* loop)
{
	Socket* soc = static_cast<Socket*>(p);
	eventloop* l = static_cast<eventloop*>(loop);
	int r = soc->accept(nullptr);
	SONE_LOG_TRACE() << "new fd = " << r;
	dispatcher* d = new dispatcher(r, l);
	l->updateDispatcher(d);
	d->setCloseCallback(std::bind(closecall, l));
	d->enableRDhup();
}


int main(void)
{
	Socket s;
	InetAddress addr("127.0.0.1", 8987, false);
	s.bindAddr(addr);
	s.listen();
	
	eventloop loop;
	dispatcher d(s.getFd(), &loop);
	loop.updateDispatcher(&d);
	d.setReadCallback(std::bind(readcall, &s, &loop));
	d.enableReading();
	loop.startloop();
	return 0;
}
