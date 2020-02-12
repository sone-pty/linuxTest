#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include "eventloop.h"
#include "Socket.h"
#include "TcpConnection.h"
#include <unordered_map>
#include "eventloopThreadPool.h"

namespace sone
{

//工作线程数量
const static int THREAD_NUMS = 20;

//HTTP报文每一行的解析状态
enum class http_line_state
{
	//行完整，行出错，行不完整
	LINE_OK, LINE_ERROR, LINE_MORE
};

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
