#include "../Socket.h"
#include "test.h"
#include "../InetAddress.h"

int main(void)
{
	Socket s;
	s.setNoBlock(false);
	InetAddress addr("127.0.0.1", 0, false);
	InetAddress seraddr("127.0.0.1", 8987, false);
	s.bindAddr(addr);
	s.connect(seraddr);
	sleep(1);
	char* str = "GET /index.html HTTP/1.1\r\njk:sd\r\n\r\n";
	write(s.getFd(), str, strlen(str));
	return 0;
}
