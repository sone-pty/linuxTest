#include "HttpServer.h"
#include "HttpConnection.h"
#include <memory>

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
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		req_check_state req_state = http_conn->getReqstate();
		http_line_state line_state;

		if(!http_conn->getRequest())
			http_conn->setRequest(new HttpRequest());
		
		while((line_state = parseLine(buffer)) == http_line_state::LINE_OK || req_state == req_check_state::CHECK_CONTENT)
		{
			switch(req_state)
			{
				case req_check_state::CHECK_REQUESTLINE:
					{
						if(!parseRequestLine(buffer, conn))
						{
							//返回错误页面并关闭连接
							return;
						}
						break;
					}
				case req_check_state::CHECK_HEADER:
					break;
				case req_check_state::CHECK_CONTENT:
					break;
			}
		}

		if(line_state == http_line_state::LINE_ERROR)
		{
			//返回错误页面并关闭连接
		}
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
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
	}

	bool HttpServer::parseRequestLine(Buffer* buf, const TcpConnection::ptr& conn)
	{
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		HttpRequest* req = http_conn->getRequest();
		http_method method;
		ssize_t first_space, second_space, end;

		first_space = buf->findChar(' ');
		end = buf->findChar('\r');
		
		if(first_space < 0)
			return false;
		else
		{
			if((method = ConvertStringToMethod(buf->getDataToString(first_space))) == http_method::UNKNOW)
				return false;
			else
				req->setMethod(method);
		}

		second_space = buf->findChar(' ', first_space + 1);

		if(second_space < 0)
			return false;
		else
			req->setRequestUrl(buf->getDataToString(second_space - first_space - 1, first_space + 1));
		
		char* c = buf->peek() + end - 1;
		if(*c == '1')
			req->setVersion(http_version::HTTP11);
		else if(*c == '0' && *(c - 2) == '1')
			req->setVersion(http_version::HTTP10);
		else
			return false;

		return true;
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
