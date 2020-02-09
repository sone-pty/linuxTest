#include "TcpConnection.h"

namespace sone
{
	TcpConnection::TcpConnection(eventloop* l, int fd, const InetAddress& local_addr, const InetAddress& peer_addr)
		:_socket(new Socket(fd, AF_INET, "")), loop(l), _dispatcher(new dispatcher(fd, l)), localaddr(local_addr), peeraddr(peer_addr)
	{
		_dispatcher->setReadCallback(std::bind(&TcpConnection::handleRead, this));
		_dispatcher->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
		_dispatcher->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
		//_socket->setKeepAlive(true);
	}

	TcpConnection::~TcpConnection()
	{

	}

	void TcpConnection::connecionEstablished()
	{
		_dispatcher->enableReading();
		_dispatcher->enableRDhup();
		connection_cb(shared_from_this());
	}

	void TcpConnection::connecionDestroyed()
	{

	}

	void TcpConnection::handleRead()
	{

	}

	void TcpConnection::handleWrite()
	{

	}

	void TcpConnection::handleClose()
	{
		
	}
}
