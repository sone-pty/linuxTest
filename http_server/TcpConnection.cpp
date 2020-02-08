#include "TcpConnection.h"

namespace sone
{
	TcpConnection::TcpConnection(eventloop* l, int fd, const InetAddress& local_addr, const InetAddress& peer_addr)
		:_socket(fd), loop(l), localaddr(local_addr), peeraddr(peer_addr)
	{

	}
}
