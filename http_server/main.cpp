#include <iostream>
#include "Log.h"
#include "mutex.h"
#include "condition.h"
#include <memory>
#include <functional>
#include "Thread.h"
#include "nocopyable.h"
#include <atomic>
#include <vector>
#include <bitset>
#include "utils.h"
#include "../sysHeader.h"

using namespace std;
using namespace sone;
using namespace util;

/*
void dynamic_arg_func(const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	char* buf;
	int len = vasprintf(&buf, fmt, vl);

	if(len != -1)
		std::cout << buf << std::endl;
	free(buf);
	va_end(vl);
}
*/

int main(void) {
	/*
    sone::Logger::ptr logger(new sone::Logger);
    logger->addAppender(sone::LogAppender::ptr(new sone::StdoutLogAppender));

    sone::FileLogAppender::ptr file_appender(new sone::FileLogAppender("./logfile.log"));
    sone::LogFormatter::ptr fmt(new sone::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sone::LogLevel::ERROR);

    logger->addAppender(file_appender);
	logger->setLevel(sone::LogLevel::FATAL);
    SONE_LOG_INFO(logger) << "test macro";
    SONE_LOG_ERROR(logger) << "test macro error";
    SONE_LOG_FMT_ERROR(logger, "test macro fmt error %s", "pty");
	*/
	return 0;
}
