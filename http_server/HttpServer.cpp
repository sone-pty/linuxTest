#include "HttpServer.h"

namespace sone
{
	HttpServer::HttpServer(eventloop* loop, const InetAddress& listenaddr)
		:main_loop(loop)
	{
		//创建监听套接字
		listen_soc = new Socket();
		listen_soc->bindAddr(listenaddr);
		listen_soc->setNoDelay(true);
		listen_soc->listen();
		//创建dispatcher
		accept_dispatcher = new dispatcher(listen_soc->getFd(), main_loop);
		accept_dispatcher->setReadCallback(std::bind(&HttpServer::AcceptCallback, this));
		accept_dispatcher->enableReading();
	}

	HttpServer::~HttpServer()
	{

	}

	void HttpServer::start()
	{
		
	}

	void HttpServer::AcceptCallback()
	{
		InetAddress addr;
		int connfd = listen_soc->accept(&addr);
		if(connfd >= 0)
		{
			//
		}
	}
}
