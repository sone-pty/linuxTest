#include "HttpConnection.h"

namespace sone
{
	HttpConnection::HttpConnection(eventloop* l, int fd, const InetAddress& local_addr, const InetAddress& peer_addr)
		:TcpConnection(l, fd, local_addr, peer_addr)
	{

	}

	HttpConnection::~HttpConnection()
	{

	}

	void HttpConnection::connecionEstablished()
	{
		_dispatcher->enableReading();
		//_dispatcher->enableRDhup();
		connection_cb(shared_from_this());
	}

	void HttpConnection::connecionDestroyed()
	{

	}

	void HttpConnection::handleRead()
	{
		
	}

	void HttpConnection::handleWrite()
	{

	}

	void HttpConnection::handleClose()
	{
		
	}
}
