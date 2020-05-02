#ifndef FCGI_H_
#define FCGI_H_

#include <string>
#include "fast_cgi.h"
#include "TcpConnection.h"
#include "HttpConnection.h"
#include <map>

namespace sone
{

class FcgiConnection : public TcpConnection{
public:
	typedef std::shared_ptr<FcgiConnection> ptr;

	FcgiConnection(eventloop* loop, int fd, const InetAddress& local_addr, const InetAddress& peer_addr, int requestId = 1);
	~FcgiConnection();
	void setRequestId(int requestId) { _requestId = requestId; }
	void sendBeginRequest();
	void sendParams(const std::string& key, const std::string& value);
	void sendEndRequest();
	void readContent(Buffer *buf);

	void connecionEstablished() override;
	void connecionDestroyed() override;
private:
	void handleRead() override;
	void handleWrite() override;
	void handleClose() override;
	void makeValueBody(const std::string& key, const std::string& value, unsigned char*	bodybuffer, int* bodylen);
private:
	//fastcgi请求类型，默认为1
	int _requestId;
};

}

#endif
