#include "HttpServer.h"
#include "HttpConnection.h"
#include <memory>
#include "consts.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

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
		int connfd;
		while((connfd = listen_soc->accept(&addr)) < 0 && errno == EINTR);
		
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
		req_check_state req_state;
		http_line_state line_state;

		if(!http_conn->getRequest())
			http_conn->setRequest(new HttpRequest());

		SONE_LOG_ROOT() << buffer->getDataToString(buffer->dataLen());
		while ((req_state = http_conn->getReqstate()) == req_check_state::CHECK_CONTENT || (line_state = parseLine(buffer)) == http_line_state::LINE_OK)
		{
			//req_state = http_conn->getReqstate();
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
					{
						if(!parseHeaderLine(buffer, conn))
						{
							//返回错误页面并关闭连接
							return;
						}
						break;
					}
				case req_check_state::CHECK_CONTENT:
					{
						if(!parseContent(buffer, conn))
						{
							//返回错误页面并关闭连接
							return;
						}
						break;
					}
			}
		}

		if(line_state == http_line_state::LINE_ERROR)
		{
			//返回错误页面并关闭连接
		}

		//解析请求成功
		SONE_LOG_TRACE() << "开始解析于" << t.to_string(false) << "的请求，成功解析完成";
		
		HttpResponse resp;
		createResponse(resp, http_conn->getRequest());
		Buffer buf;
		std::string message = std::move(resp.toString());
		buf.append(message.c_str(), message.length());
		http_conn->send(&buf);
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
		int nums = connections.erase(conn->getSockfd());
		if(nums == 0)
			SONE_LOG_ERR() << "HttpServer::onClose()-----connections.erase()失败";
	}

	void HttpServer::createResponse(HttpResponse& resp, HttpRequest* req)
	{
		char buf[BUFSIZE];
		resp.setVersion(req->getVersion());
		resp.setHeader("Content-Type", "text/html;charset=utf-8");
		resp.setHeader("Server", SERVER_VERSION);
		//resp.setHeader("Date", util::Timestamp().to_string(false));
		std::string request_url = std::move(req->getRequestUrl());
		std::string complete_url(WEB_ROOT);
		complete_url.append(request_url);
		
		int fd = ::open(complete_url.c_str(), O_RDONLY);
		if(fd == -1)
		{
			SONE_LOG_ERR() << "createResponse---open() failed";
			if(errno == EACCES)
				resp.setRespState(http_resp_state::Not_Found);
			else
				resp.setRespState(http_resp_state::Internal_Server_Error);
		}
		else
		{
			std::string s;
			s.reserve(BUFSIZE);
			ssize_t len;
			while((len = ::read(fd, buf, BUFSIZE)) != 0)
				s.append(buf, buf + len);
			resp.setContent(std::move(s));
		}
		resp.setRespState(http_resp_state::OK);
	}

	bool HttpServer::parseContent(Buffer* buf, const TcpConnection::ptr& conn)
	{
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		HttpRequest* req = http_conn->getRequest();
		req->setContent(buf->getDataToString(buf->dataLen()));
		buf->moveLow(buf->dataLen());
		//以防keep-alive，重置解析状态
		http_conn->setReqstate(req_check_state::CHECK_REQUESTLINE);
		return true;
	}

	bool HttpServer::parseHeaderLine(Buffer* buf, const TcpConnection::ptr& conn)
	{
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		HttpRequest* req = http_conn->getRequest();
		ssize_t index, end;
		std::string key, val;
		http_headers header;

		end = buf->findChar('\r');
		index = buf->findChar(':', 0, end);
		
		if(index < 0)
		{
			//空行
			if(end == 0)
			{
				http_conn->setReqstate(req_check_state::CHECK_CONTENT);
				buf->moveLow(2);
				return true;
			}
			else
				return false;
		}
		else
		{
			key = buf->getDataToString(index);
			if((header = ConvertStringToHeader(key)) == http_headers::UNKNOW)
				return false;
			else
			{
				val = buf->getDataToString(end - index - 1, index + 1);
				if(val[0] == ' ')
					val.erase(val.begin());
				req->setHeader(ConvertHeaderToString(header), val);
				buf->moveLow(end + 2);
				return true;
			}
		}
	}

	bool HttpServer::parseRequestLine(Buffer* buf, const TcpConnection::ptr& conn)
	{
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		HttpRequest* req = http_conn->getRequest();
		http_method method;
		ssize_t first_space, second_space, end;

		end = buf->findChar('\r');
		first_space = buf->findChar(' ', 0, end);
		
		if(first_space < 0)
			return false;
		else
		{
			if((method = ConvertStringToMethod(buf->getDataToString(first_space))) == http_method::UNKNOW)
				return false;
			else
				req->setMethod(method);
		}

		second_space = buf->findChar(' ', first_space + 1, end);

		if(second_space < 0)
			return false;
		else
		{
			//需要确定有没有请求参数
			ssize_t index = buf->findChar('?', first_space + 1, end);
			if(index < 0)
				req->setRequestUrl(util::URLDecode(buf->getDataToString(second_space - first_space - 1, first_space + 1)));
			else
			{
				req->setRequestUrl(util::URLDecode(buf->getDataToString(index - first_space - 1, first_space + 1)));
				//解析参数，需要先解析后解码
				std::string params = buf->getDataToString(second_space - index - 1, index + 1), temp;
				size_t split = 0, lastsplit = -1, eq = 0;

				while((split = params.find('&', lastsplit + 1)) != std::string::npos)
				{
					temp = params.substr(lastsplit + 1, split - lastsplit - 1);
					if((eq = temp.find('=')) != std::string::npos)
						req->setParam(util::URLDecode(temp.substr(0, eq)), util::URLDecode(temp.substr(eq + 1, temp.length() - eq - 1)));
					else
						return false;
					lastsplit = split;
				}
				//最后一对参数
				temp = params.substr(lastsplit + 1);
				if((eq = temp.find('=')) != std::string::npos)
					req->setParam(util::URLDecode(temp.substr(0, eq)), util::URLDecode(temp.substr(eq + 1, temp.length() - eq - 1)));
				else
					return false;
			}
		}
		
		char* c = buf->peek() + end - 1;
		if(*c == '1')
			req->setVersion(http_version::HTTP11);
		else if(*c == '0' && *(c - 2) == '1')
			req->setVersion(http_version::HTTP10);
		else
			return false;

		//如果解析成功，将buf中low后移
		buf->moveLow(end + 2);
		//改变解析状态
		http_conn->setReqstate(req_check_state::CHECK_HEADER);
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
