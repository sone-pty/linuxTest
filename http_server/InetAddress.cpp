#include "InetAddress.h"
#include "Log.h"

namespace sone
{
	InetAddress::InetAddress()
	{}
	
	InetAddress::InetAddress(uint16_t port, bool isloop, bool ipv6)
	{
		if(ipv6)
		{
			memset(&ipv6addr, 0, sizeof(ipv6addr));
			ipv6addr.sin6_family = AF_INET6;
			ipv6addr.sin6_addr = isloop ? in6addr_loopback : in6addr_any;
			ipv6addr.sin6_port = htons(port);
			_type = IPV6;
		}
		else
		{
			memset(&ipv4addr, 0, sizeof(ipv4addr));
			ipv4addr.sin_family = AF_INET;
			ipv4addr.sin_addr.s_addr = isloop ? htonl(INADDR_LOOPBACK) : htonl(INADDR_ANY);
			ipv4addr.sin_port = htons(port);
			_type = IPV4;
		}
	}

	InetAddress::InetAddress(const char* ip, uint16_t port, bool ipv6)
	{
		int res;

		if(ipv6)
		{
			memset(&ipv6addr, 0, sizeof(ipv6addr));
			ipv6addr.sin6_family = AF_INET6;
			res = inet_pton(AF_INET6, ip, &ipv6addr.sin6_addr);
			if(res == 0)
			{
				SONE_LOG_ROOT() << "不正确的地址，未能将字符串转化地址";
			}
			else if(res == -1 && errno == EAFNOSUPPORT)
				SONE_LOG_ROOT() << "不正确的SA_FAMILY参数";
			ipv6addr.sin6_port = htons(port);
			_type = IPV6;
		}
		else
		{
			memset(&ipv4addr, 0, sizeof(ipv4addr));
			ipv4addr.sin_family = AF_INET;
			res = inet_pton(AF_INET, ip, &ipv4addr.sin_addr);
			if(res == 0)
			{
				SONE_LOG_ROOT() << "不正确的地址，未能将字符串转化地址";
			}
			else if(res == -1 && errno == EAFNOSUPPORT)
				SONE_LOG_ROOT() << "不正确的SA_FAMILY参数";
			ipv4addr.sin_port = htons(port);
			_type = IPV4;
		}
	}

	std::string InetAddress::ip_to_string() const
	{
		char buf[64];
		memset(buf, 0, 64);
		if(_type == IPV4)
			inet_ntop(AF_INET, &ipv4addr.sin_addr, buf, 64);
		else
			inet_ntop(AF_INET6, &ipv6addr.sin6_addr, buf, 64);
		return buf;
	}
}
