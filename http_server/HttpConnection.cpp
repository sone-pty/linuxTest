#include "HttpConnection.h"
#include "consts.h"
#include "Log.h"

namespace sone
{
	HttpConnection::HttpConnection(eventloop* l, int fd, const InetAddress& local_addr, const InetAddress& peer_addr, TimerHeap* th)
		:TcpConnection(l, fd, local_addr, peer_addr), _theap(th)
	{

	}

	HttpConnection::~HttpConnection()
	{
		SONE_LOG_TRACE() << "delete Http Connection";
		delete _tim;
	}

	void HttpConnection::connecionEstablished()
	{
		_dispatcher->enableReading();
		connection_cb(shared_from_this());
	}

	void HttpConnection::connecionDestroyed()
	{

	}

	void HttpConnection::handleRead()
	{
		_theap->getLock().lock();
		if(_tim && _tim->getIndex() == -2)
			return;
		else if(_tim && _tim->getIndex() >= 0)
		{
			//定时器延时并调整堆序
			_tim->setExpire(time(nullptr) + HTTP_KEEPALIVE_TIME);
			_theap->down(_tim->getIndex());
		}
		_theap->getLock().unlock();
		ssize_t n = input_buffer.read(_socket->getFd());
		if(n > 0)
			message_cb(shared_from_this(), &input_buffer, util::Timestamp());
		else if(n == 0)
		{
			//关闭连接
		}
		else
			SONE_LOG_ERR() << "handleRead()--read() failed";
	}

	void HttpConnection::handleWrite()
	{
		ssize_t len = ::write(_socket->getFd(), output_buffer.peek(), output_buffer.dataLen());
		if(len == output_buffer.dataLen())
		{
			//handleClose();
			if(_request->getHeader("Connection") == "close" || _request->getVersion() == http_version::HTTP10)
				handleClose();
			else
			{
				_dispatcher->disableWriting();
				if(!_tim)
				{
					Timer* tim = new Timer(HTTP_KEEPALIVE_TIME);
					tim->setCb(std::bind(&HttpConnection::handleTimerCb, this));
					_theap->pushTimer(tim);
					this->_tim = tim;
				}
			}
		}
		else
			output_buffer.moveLow(len);
	}

	void HttpConnection::handleClose()
	{
		SONE_LOG_TRACE() << "handleClose";
		loop->removeDispatcher(_dispatcher.get());
		if(close_cb)
			close_cb(shared_from_this());
	}

	void HttpConnection::handleTimerCb()
	{
		loop->runInLoop(std::bind(&HttpConnection::handleClose, this));
	}

	void HttpConnection::send(Buffer* buf)
	{
		ssize_t len = ::write(_socket->getFd(), buf->peek(), buf->dataLen());
		if(len < buf->dataLen())
		{
			_dispatcher->enableWriting();
			output_buffer.append(buf->peek() + len, buf->dataLen() - len);
		}
		//一次write发送完所有数据
		else if(_request->getHeader("Connection") == "close" || _request->getVersion() == http_version::HTTP10)
			handleClose();
		//keep-alive
		else
		{
			if(!_tim)
			{
				Timer* tim = new Timer(HTTP_KEEPALIVE_TIME);
				tim->setCb(std::bind(&HttpConnection::handleTimerCb, this));
				_theap->pushTimer(tim);
				this->_tim = tim;
			}
		}
	}
}
