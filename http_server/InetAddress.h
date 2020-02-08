#ifndef INETADDR_H_
#define INETADDR_H_

#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <string.h>

namespace sone
{

class InetAddress
{
public:
	typedef enum
	{
		IPV4,
		IPV6
	}addr_type;
	InetAddress();
	//没有提供IP地址的构造方法(使用环回地址或者本地地址)
	explicit InetAddress(uint16_t port, bool isloop, bool ipv6);
	//提供IP地址的构造方法
	explicit InetAddress(const char* ip, uint16_t port = 0, bool ipv6 = false);
	//直接以结构体为参数构造
	explicit InetAddress(const struct sockaddr_in& addr)
		:ipv4addr(addr), _type(IPV4) {}
	explicit InetAddress(const struct sockaddr_in6& addr)
		:ipv6addr(addr), _type(IPV6) {}
	//地址类型
	addr_type Addrtype() const { return _type; }
	//返回地址结构(不可修改)
	const struct sockaddr* Sockaddr() const { return (const sockaddr*)(&ipv4addr); }
	//返回地址结构，非const
	struct sockaddr* writableSockaddr() { return (sockaddr*)(&ipv4addr); }
	//返回地址结构的family
	sa_family_t family() const { return ipv4addr.sin_family; }
	//字符串输出
	std::string ip_to_string() const;
	std::string port_to_string() const { return std::to_string(ntohs(ipv4addr.sin_port)); }
private:
	//地址共用体
	union
	{
		sockaddr_in ipv4addr;
		sockaddr_in6 ipv6addr;
	};
	//地址类型
	addr_type _type;
};

}

#endif
