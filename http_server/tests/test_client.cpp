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
	write(s.getFd(), "hello", 6);
	return 0;
}
