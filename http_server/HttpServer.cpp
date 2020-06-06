#include "HttpServer.h"
#include "HttpConnection.h"
#include "FcgiConnection.h"
#include <memory>
#include "consts.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
 
#include <iostream>
#include <sstream>
#include <regex>

namespace sone
{
	HttpServer::HttpServer(eventloop* loop, const InetAddress& listenaddr, TimerHeap* th)
		:main_loop(loop), _threadpool(new eventloopThreadPool(THREAD_NUMS)), _theap(th), _cache(LRUCACHE_CAPCITY)
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
			TcpConnection::ptr conn = TcpConnection::ptr(new HttpConnection(ioLoop, connfd, localaddr, addr, _theap));
			
			_lock.lock();
			connections.push_back(conn);
			_lock.unlock();
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

	void HttpServer::retErrResponse(const TcpConnection::ptr& conn)
	{
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		HttpResponse resp;
		resp.setVersion(http_conn->getRequest()->getVersion());
		resp.setHeader("Content-Type", "text/html;charset=utf-8");
		resp.setHeader("Server", SERVER_VERSION);
		resp.setHeader("Date", util::Timestamp().toGMTString());
		resp.setContent(ERROR_REQ_PAGE);
		resp.setHeader("Content-Length", std::to_string(ERROR_REQ_PAGE.length()));
		Buffer buf;
		buf.append(resp.toString());
		//将Connection首部设置为close，使得能够直接关闭连接
		http_conn->getRequest()->setHeader("Connection", "close");
		http_conn->send(&buf);
	}

	void HttpServer::onMessage(const TcpConnection::ptr& conn, Buffer* buffer, util::Timestamp t)
	{
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		req_check_state req_state;
		http_line_state line_state;
		bool flag = false;

		if (!http_conn->getRequest())
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
							retErrResponse(http_conn);
							return;
						}
						break;
					}
				case req_check_state::CHECK_HEADER:
					{
						if(!parseHeaderLine(buffer, conn))
						{
							//返回错误页面并关闭连接
							retErrResponse(http_conn);
							return;
						}
						break;
					}
				case req_check_state::CHECK_CONTENT:
					{
						if(!parseContent(buffer, conn))
						{
							//返回错误页面并关闭连接
							retErrResponse(http_conn);
							return;
						}
						flag = true;
						break;
					}
			}
		}

		if(line_state == http_line_state::LINE_ERROR)
		{
			//返回错误页面并关闭连接
			retErrResponse(http_conn);
			return;
		}
		else if(line_state == http_line_state::LINE_MORE && !flag)
			return;

		//解析请求成功
		//SONE_LOG_TRACE() << "开始解析于" << t.to_string(false) << "的请求，成功解析完成";
		
		HttpResponse resp;
		std::regex r(".*\\.php", std::regex::icase);
		std::smatch sma;
		std::string url = http_conn->getRequest()->getRequestUrl();
		Buffer buf;
		if (std::regex_match(url, sma, r))
		{
			Buffer* tmp = createPhpResponse(http_conn->getRequest());
			//获取动态资源成功
			if(tmp)
			{
				buf.append("HTTP/1.1 200 OK\r\n");
				buf.append(tmp->peek(), tmp->dataLen());
				delete tmp;
				http_conn->send(&buf);
			}
			//获取动态资源失败
			else
			{
				buf.append("HTTP/1.1 404 NOT FOUND\r\n");
				buf.append("Content-Length: " + std::to_string(NOT_FOUND_PAGE.length()) + "\r\n\r\n");
				buf.append(NOT_FOUND_PAGE);
				http_conn->send(&buf);
			}
			return;
		}
		else
			createResponse(resp, http_conn->getRequest(), t);
		std::string message = std::move(resp.toString());
		buf.append(message.c_str(), message.length());
		http_conn->send(&buf);
	}

	void HttpServer::onConnection(const TcpConnection::ptr& conn)
	{
		if(conn)
		{
			//SONE_LOG_TRACE() << "new conn";
		}
	}

	void HttpServer::onClose(const TcpConnection::ptr& conn)
	{
		_lock.lock();
		std::list<TcpConnection::ptr>::iterator iter;
		for(iter = connections.begin();iter != connections.end();++iter)
		{
			if(*iter == conn)
				break;
		}
		connections.erase(iter);
		_lock.unlock();
	}

	void HttpServer::createResponse(HttpResponse& resp, HttpRequest* req, util::Timestamp req_time)
	{
		std::string request_url = std::move(req->getRequestUrl());
		std::string complete_url(WEB_ROOT);
		complete_url.append(request_url);
		util::Timestamp ts(0, 0);
		
		int fd = ::open(complete_url.c_str(), O_RDONLY);
		char buf[BUFSIZE];
		
		resp.setVersion(req->getVersion());
		//resp.setHeader("Content-Type", "text/html;charset=utf-8");
		resp.setHeader("Server", SERVER_VERSION);
		resp.setHeader("Date", util::Timestamp().toGMTString());
		//检查请求中的协商缓存
		std::string modified_time = req->getHeader("If-Modified-Since");

		struct stat s;
		int ret = ::fstat(fd, &s);
		if(ret < 0)
			SONE_LOG_ERR() << "fstat(...) failed";
		ts.setSecond(s.st_mtim.tv_sec);

		if(modified_time != "")
		{
			//检查最后一次修改时间
			if(fd != -1)
			{
				std::string tsGmt = ts.toGMTString();
				//协商缓存成功，直接返回
				if(util::compareGMTStr(modified_time, tsGmt))
				{
					resp.setRespState(http_resp_state::Not_Modified);
					return;
				}
			}
		}
		
		//第一次访问该资源或者资源失效的情况
		resp.setHeader("Last-Modified", ts.toGMTString());
		req_time.setSecond(req_time.getSecond() + HTTP_CACHE_MAX_AGE);
		resp.setHeader("Expires", req_time.toGMTString());
		resp.setHeader("Cache-Control", "max-age=" + std::to_string(HTTP_CACHE_MAX_AGE));
		if(fd == -1)
		{
			resp.setRespState(http_resp_state::Not_Found);
			resp.setContent(NOT_FOUND_PAGE);
			resp.setHeader("Content-Length", std::to_string(NOT_FOUND_PAGE.length()));
		}
		else
		{
			//对应的资源
			/*
			std::string s;
			s.reserve(BUFSIZE);
			ssize_t len;
			while((len = ::read(fd, buf, BUFSIZE)) != 0)
				s.append(buf, buf + len);
			*/
			if(!_cache.getValue(complete_url))
				_cache.setValue(complete_url);
			std::string s(_cache.getValue(complete_url), _cache.getSize(complete_url));
			//设置Content-type
			std::regex r("\\.([a-z]+)", std::regex::icase);
			std::smatch sma;
			std::string type;
			if(std::regex_search(request_url, sma, r))
			{
				type = sma.str(1);
				if(strcasecmp(type.c_str(), "html") == 0)
					resp.setHeader("Content-Type", "text/html;charset=utf-8");
				else if(strcasecmp(type.c_str(), "png") == 0)
					resp.setHeader("Content-Type", "image/png");
				else if(strcasecmp(type.c_str(), "jpg") == 0)
					resp.setHeader("Content-Type", "image/jpeg");
				else if(strcasecmp(type.c_str(), "js") == 0)
					resp.setHeader("Content-Type", "application/javascript");
				else if(strcasecmp(type.c_str(), "css") == 0)
					resp.setHeader("Content-Type", "text/css");
				else
					resp.setHeader("Content-Type", "text/plain");
			}
			//gzip压缩
			s = gzipCompress(s);
			resp.setContent(std::move(s));
			//move之后s不可用
			resp.setHeader("Content-Length", std::to_string(resp.getContent().length()));
			resp.setHeader("Content-Encoding", "gzip");
			resp.setRespState(http_resp_state::OK);
			::close(fd);
		}
	}

	Buffer* HttpServer::createPhpResponse(HttpRequest* req)
	{
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		InetAddress peer_addr("127.0.0.1", 9000, false);
		::connect(fd, peer_addr.Sockaddr(), sizeof(sockaddr_in));
		InetAddress local_addr(util::getAddrbyFdV4(fd));
		std::string body = util::URLDecode(req->getContent());

		FcgiConnection fcgi_conn(nullptr, fd, local_addr, peer_addr);

		fcgi_conn.sendBeginRequest();
		fcgi_conn.sendParams("SCRIPT_FILENAME", WEB_ROOT + req->getRequestUrl());
		if(req->getVersion() == http_version::HTTP11)
			fcgi_conn.sendParams("SERVER_PROTOCOL", "HTTP/1.1");
		else
			fcgi_conn.sendParams("SERVER_PROTOCOL", "HTTP/1.0");
		if(req->getMethod() == http_method::GET)
			fcgi_conn.sendParams("REQUEST_METHOD", "GET");
		else
			fcgi_conn.sendParams("REQUEST_METHOD", "POST");
		if(req->getMethod() == http_method::GET)
			fcgi_conn.sendParams("QUERY_STRING", req->getQueryString());
		else
		{
			fcgi_conn.sendParams("CONTENT_LENGTH", std::to_string(body.length()));
			fcgi_conn.sendParams("CONTENT_TYPE", "application/x-www-form-urlencoded");
		}
		fcgi_conn.sendEndRequest();
		
		if(req->getMethod() == http_method::POST)
			fcgi_conn.sendContentHeader(req->getContent(), req->getContent().length());
		Buffer* buf = new Buffer();
		fcgi_conn.readContent(buf);
		
		if(buf->dataLen())
			return buf;
		else
		{
			delete buf;
			return nullptr;
		}
	}

	bool HttpServer::parseContent(Buffer* buf, const TcpConnection::ptr& conn)
	{
		HttpConnection::ptr http_conn = std::dynamic_pointer_cast<HttpConnection>(conn);
		HttpRequest* req = http_conn->getRequest();
		req->setContent(buf->getDataToString(buf->dataLen()));
		//post提交的表单参数（默认application/x-www-form-urlencoded格式）
		std::string params = req->getContent(), temp;
		if(req->getMethod() == http_method::POST && req->getHeader("Content-Type") == "application/x-www-form-urlencoded")
		{
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
		buf->moveLow(buf->dataLen());
		//以防keep-alive，重置解析状态
		http_conn->setReqstate(req_check_state::CHECK_REQUESTLINE);
		return true;
	}

	std::string HttpServer::gzipCompress(const std::string& s)
	{
		std::stringstream ss, dest;
		ss << s;
		boost::iostreams::filtering_ostream out;
		out.push(boost::iostreams::gzip_compressor());
		out.push(dest);
		boost::iostreams::copy(ss, out);
		return dest.str();
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
				req->setQueryString(util::URLDecode(buf->getDataToString(second_space - index - 1, index + 1)));
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
