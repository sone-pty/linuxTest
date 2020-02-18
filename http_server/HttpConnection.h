#ifndef HTTPCONNECTION_H_
#define HTTPCONNECTION_H_

#include "TcpConnection.h"
#include "http.h"

namespace sone
{

class HttpConnection : public TcpConnection{
public:
	typedef std::shared_ptr<HttpConnection> ptr;
	HttpConnection(eventloop *l, int fd, const InetAddress &local_addr, const InetAddress &peer_addr);
	virtual ~HttpConnection();
	void connecionEstablished() override;
	void connecionDestroyed() override;
	//http
	HttpRequest* getRequest() const { return _request.get(); }
	void setRequest(HttpRequest* req) { _request.reset(req); }
	req_check_state getReqstate() const { return _state;  }
	void setReqstate(req_check_state state) { _state = state; }
	void send(Buffer* buf);
protected:
	void handleRead() override;
	void handleWrite() override;
	void handleClose() override;
private:
	//http请求
	std::unique_ptr<HttpRequest> _request;
	//请求解析状态
	req_check_state _state = req_check_state::CHECK_REQUESTLINE;
};

}

#endif
