#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include "eventloop.h"
#include "dispatcher.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Buffer.h"
#include "nocopyable.h"

namespace sone
{

class TcpConnection : public nocopyable, public std::enable_shared_from_this<TcpConnection>{
public:
	typedef std::shared_ptr<TcpConnection> ptr;
	typedef std::function<void(const TcpConnection::ptr&)> ConnectionCallback;
	typedef std::function<void(const TcpConnection::ptr&)> CloseCallback;
	typedef std::function<void(const TcpConnection::ptr&, Buffer*, util::Timestamp)> MessageCallback;

	TcpConnection(eventloop* l, int fd, const InetAddress& local_addr, const InetAddress& peer_addr);
	virtual ~TcpConnection();
	void setConnectionCallback(ConnectionCallback cb) { connection_cb = std::move(cb); }
    void setCloseCallback(CloseCallback cb) { close_cb = std::move(cb);  }
    void setMessageCallback(MessageCallback cb) { message_cb = std::move(cb);  }
	virtual void connecionEstablished() = 0;
	virtual void connecionDestroyed() = 0;
protected:
	virtual void handleRead() = 0;
	virtual void handleWrite() = 0;
	virtual void handleClose() = 0;
protected:
	//对应的套接字
	std::unique_ptr<Socket> _socket;
	//所属的eventloop
	eventloop* loop;
	//对应的dispatcher
	std::unique_ptr<dispatcher> _dispatcher;
	//本地地址
	InetAddress localaddr;
	//对端地址
	InetAddress peeraddr;
	//输入输出缓冲区
	Buffer input_buffer;
	Buffer output_buffer;
protected:
	ConnectionCallback connection_cb;
	CloseCallback close_cb;
	MessageCallback message_cb;
};

}

#endif
