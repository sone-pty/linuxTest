#include "HttpServer.h"
#include "HttpConnection.h"

namespace sone
{
	HttpServer::HttpServer(eventloop* loop, const InetAddress& listenaddr)
		:main_loop(loop), _threadpool(new eventloopThreadPool(THREAD_NUMS))
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
		_threadpool->start();
	}

	void HttpServer::AcceptCallback()
	{
		InetAddress addr;
		int connfd = listen_soc->accept(&addr);
		if(connfd >= 0)
		{
			eventloop* ioLoop = _threadpool->getLoop(connfd);
			//创建TcpConnection
			InetAddress localaddr(util::getAddrbyFdV4(connfd));
			TcpConnection::ptr conn = TcpConnection::ptr(new HttpConnection(ioLoop, connfd, localaddr, addr));
			if(connections.find(connfd) != connections.end())
			{
				SONE_LOG_ERR() << "connections已经存在该连接，无法添加";
				abort();
			}
			connections[connfd] = conn;
			conn->setConnectionCallback(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
			conn->setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			conn->setCloseCallback(std::bind(&HttpServer::onClose, this, std::placeholders::_1));
			//连接创建完成后往loop中塞入回调
			ioLoop->runInLoop(std::bind(&TcpConnection::connecionEstablished, conn));
		}
	}

	eventloop* HttpServer::getLoop()
	{
		return main_loop;
	}

	void HttpServer::onMessage(const TcpConnection::ptr& conn, Buffer* buffer, util::Timestamp t)
	{
		
	}

	void HttpServer::onConnection(const TcpConnection::ptr& conn)
	{
		if(conn)
		{
			SONE_LOG_TRACE() << "new conn";
		}
	}

	void HttpServer::onClose(const TcpConnection::ptr& conn)
	{

	}

	http_line_state HttpServer::parseLine(Buffer* buf)
	{
		ssize_t pos = buf->findChar('\r');

		if(pos == -1)
			return http_line_state::LINE_MORE;
		else
		{
			if(pos + 1 == buf->dataLen())
				return http_line_state::LINE_MORE;
			else if(*(buf->peek() + pos + 1) == '\n')
				return http_line_state::LINE_OK;
			else
				return http_line_state::LINE_ERROR;
		}
	}
}
