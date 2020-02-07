#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include "eventloop.h"
#include "Socket.h"
#include "TcpConnection.h"
#include <unordered_map>

namespace sone
{

class HttpServer{
public:
	HttpServer(eventloop* loop, const InetAddress& listenaddr);
	~HttpServer();
	eventloop* getLoop();
	void start();
private:
	//accept对应的处理回调
	void AcceptCallback();
private:
	//主线程eventloop
	eventloop* main_loop;
	//监听套接字
	Socket* listen_soc;
	//用于连接的dispatcher
	dispatcher* accept_dispatcher;
	//保存TCPConnection的map
	std::unordered_map<int, TcpConnection::ptr> connections;
};

}

#endif
