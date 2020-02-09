#ifndef HTTPCONNECTION_H_
#define HTTPCONNECTION_H_

#include "TcpConnection.h"
#include "HttpParser.h"

namespace sone
{

class HttpConnection : public TcpConnection{
public:
	HttpConnection(eventloop* l, int fd, const InetAddress& local_addr, const InetAddress& peer_addr);
	virtual ~HttpConnection();
	void connecionEstablished() override;
	void connecionDestroyed() override;
protected:
	void handleRead() override;
	void handleWrite() override;
	void handleClose() override;
private:
	std::unique_ptr<HttpParser> _parser;
};

}

#endif
