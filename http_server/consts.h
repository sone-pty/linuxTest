#ifndef CONSTS_H_
#define CONSTS_H_

#include <string>

namespace sone
{
	const std::string SERVER_VERSION = "sone/1.0";
	const std::string WEB_ROOT = "/home/sone/web_root";
	const int BUFSIZE = 8192;
	const std::string NOT_FOUND_PAGE = "<html><title>404 Not Found</title><body><h1>Not Found!</h1></body></html>";
	const std::string ERROR_REQ_PAGE = "<html><title>400 Bad Request</title><body><h1>Bad Request!</h1></body></html>";
} // namespace sone

#endif
