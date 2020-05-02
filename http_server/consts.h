#ifndef CONSTS_H_
#define CONSTS_H_

#include <string>

namespace sone
{
	//服务器版本
	const std::string SERVER_VERSION = "sone/1.0";
	//资源根目录
	const std::string WEB_ROOT = "/home/sone/web_root";
	//缓冲区大小
	const int BUFSIZE = 8192;
	//404页面
	const std::string NOT_FOUND_PAGE = "<html><title>404 Not Found</title><body><h1>Not Found!</h1></body></html>";
	//400出错页面
	const std::string ERROR_REQ_PAGE = "<html><title>400 Bad Request</title><body><h1>Bad Request!</h1></body></html>";
	//HTTP静态资源缓存时间（相对时间）
	const unsigned long HTTP_CACHE_MAX_AGE = 86400;
	//定时器周期（单位s）
	//const unsigned long EXPIRE_TIME = 5;
	//http-keep-alive时间（单位s）
	//const unsigned long HTTP_KEEPALIVE_TIME = 10;
} // namespace sone

#endif
