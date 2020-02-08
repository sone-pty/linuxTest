#include "TcpConnection.h"

namespace sone
{
	TcpConnection::TcpConnection(eventloop* l, int fd, const InetAddress& local_addr, const InetAddress& peer_addr)
		:_socket(new Socket(fd, AF_INET)), loop(l), localaddr(local_addr), peeraddr(peer_addr)
	{

	}
}
