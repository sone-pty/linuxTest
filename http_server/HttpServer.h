#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include "eventloop.h"
#include "Socket.h"
#include "TcpConnection.h"
#include <unordered_map>
#include "eventloopThreadPool.h"
#include "http.h"

namespace sone
{

//工作线程数量
const static int THREAD_NUMS = 8;

class HttpServer{
public:
	HttpServer(eventloop* loop, const InetAddress& listenaddr);
	~HttpServer();
	eventloop* getLoop();
	void start();
private:
	void AcceptCallback();
	void onConnection(const TcpConnection::ptr& conn);
	void onMessage(const TcpConnection::ptr& conn, Buffer* buffer, util::Timestamp t);
	void onClose(const TcpConnection::ptr& conn);
private:
	/*解析HTTP请求*/
	http_line_state parseLine(Buffer* buf);
	bool parseRequestLine(Buffer* buf, const TcpConnection::ptr& conn);
	bool parseHeaderLine(Buffer* buf, const TcpConnection::ptr& conn);
	bool parseContent(Buffer* buf, const TcpConnection::ptr& conn);
	//构造响应
	void createResponse(HttpResponse& resp, HttpRequest* req);
private:
	//主线程eventloop
	eventloop* main_loop;
	//监听套接字
	Socket* listen_soc;
	//用于连接的dispatcher
	dispatcher* accept_dispatcher;
	//保存TCPConnection的map
	std::unordered_map<int, TcpConnection::ptr> connections;
	//线程池
	std::unique_ptr<eventloopThreadPool> _threadpool;
};

}

#endif
