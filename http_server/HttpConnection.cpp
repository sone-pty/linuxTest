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
		ssize_t n = input_buffer.read(_socket->getFd());
		if(n > 0)
			message_cb(shared_from_this(), &input_buffer, util::Timestamp());
		else if(n == 0)
			handleClose();
		else
			SONE_LOG_ERR() << "handleRead()--read() failed";
	}

	void HttpConnection::handleWrite()
	{

	}

	void HttpConnection::handleClose()
	{
		loop->removeDispatcher(_dispatcher.get());
		if(close_cb)
			close_cb(shared_from_this());
	}

	void HttpConnection::send(Buffer* buf)
	{
		
	}
}
