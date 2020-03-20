#include "../Socket.h"
#include "test.h"
#include "../InetAddress.h"

int main(void)
{
	Socket s;
	InetAddress addr("127.0.0.1", 8987, false);
	s.bindAddr(addr);
	s.listen();
	s.setNoBlock(false);
	char buf[8192];
	memset(buf, 0, 8192);
	int fd = s.accept(nullptr);
	shutdown(fd, SHUT_WR);
	int ret = read(fd, buf, 8192);
	if(ret < 0)
	{
		perror("read()-->");
		return -1;
	}
	else
	{
		cout << buf << endl;
	}
	return 0;
}
