#ifndef __SONE_LOG_H__
#define __SONE_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include <sys/syscall.h>
#include <unistd.h>
#include "mutex.h"
#include "Singleton.h"
#include "utils.h"

/**
 *  使用流式方式将日志级别level的日志写入到logger
 */
#define SONE_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        sone::LogEventWrap(sone::LogEvent::ptr(new sone::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, util::getTid(),\
                getpid(), time(0), "UNKNOW"))).getSS()
/**
 *  使用流式方式将日志级别debug的日志写入到logger
 */
#define SONE_LOG_DEBUG(logger) SONE_LOG_LEVEL(logger, sone::LogLevel::DEBUG)

/**
 *  使用流式方式将日志级别info的日志写入到logger
 */
#define SONE_LOG_INFO(logger) SONE_LOG_LEVEL(logger, sone::LogLevel::INFO)

/**
 *  使用流式方式将日志级别warn的日志写入到logger
 */
#define SONE_LOG_WARN(logger) SONE_LOG_LEVEL(logger, sone::LogLevel::WARN)

/**
 *  使用流式方式将日志级别error的日志写入到logger
 */
#define SONE_LOG_ERROR(logger) SONE_LOG_LEVEL(logger, sone::LogLevel::ERROR)

/**
 *  使用流式方式将日志级别fatal的日志写入到logger
 */
#define SONE_LOG_FATAL(logger) SONE_LOG_LEVEL(logger, sone::LogLevel::FATAL)

/**
 *  使用格式化方式将日志级别level的日志写入到logger
 */
#define SONE_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sone::LogEventWrap(sone::LogEvent::ptr(new sone::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, util::getTid(),\
                getpid(), time(0), "UNKNOW"))).getEvent()->format(fmt, __VA_ARGS__)

/*
 *  使用格式化方式将日志级别debug的日志写入到logger
 */
#define SONE_LOG_FMT_DEBUG(logger, fmt, ...) SONE_LOG_FMT_LEVEL(logger, sone::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 *  使用格式化方式将日志级别info的日志写入到logger
 */
#define SONE_LOG_FMT_INFO(logger, fmt, ...)  SONE_LOG_FMT_LEVEL(logger, sone::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 *  使用格式化方式将日志级别warn的日志写入到logger
 */
#define SONE_LOG_FMT_WARN(logger, fmt, ...)  SONE_LOG_FMT_LEVEL(logger, sone::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 *  使用格式化方式将日志级别error的日志写入到logger
 */
#define SONE_LOG_FMT_ERROR(logger, fmt, ...) SONE_LOG_FMT_LEVEL(logger, sone::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 *  使用格式化方式将日志级别fatal的日志写入到logger
 */
#define SONE_LOG_FMT_FATAL(logger, fmt, ...) SONE_LOG_FMT_LEVEL(logger, sone::LogLevel::FATAL, fmt, __VA_ARGS__)

//root日志器
#define SONE_LOG_ROOT() SONE_LOG_DEBUG(sone::Singleton<sone::LoggerManager>::getInstance()->getRoot())
#define SONE_LOG_FMT_ROOT(fmt, ...) SONE_LOG_FMT_DEBUG(sone::Singleton<sone::LoggerManager>::getInstance()->getRoot(), fmt, ...)

//trace日志器
#define SONE_LOG_TRACE() SONE_LOG_DEBUG(sone::Singleton<sone::LoggerManager>::getInstance()->getTraceLog())
#define SONE_LOG_FMT_TRACE(fmt, ...) SONE_LOG_FMT_DEBUG(sone::Singleton<sone::LoggerManager>::getInstance()->getTraceLog(), fmt, ...)

//error日志器
#define SONE_LOG_ERR() SONE_LOG_DEBUG(sone::Singleton<sone::LoggerManager>::getInstance()->getErrorLog())
#define SONE_LOG_FMT_ERR(fmt, ...) SONE_LOG_FMT_DEBUG(sone::Singleton<sone::LoggerManager>::getInstance()->getErrorLog(), fmt, ...)

namespace sone {

class Logger;

/**
 *  日志级别
 */
class LogLevel {
public:
    enum Level {
        // 未知级别
        UNKNOW = 0,
        // DEBUG 级别
        DEBUG = 1,
        // INFO 级别
        INFO = 2,
        // WARN 级别
        WARN = 3,
        // ERROR 级别
        ERROR = 4,
        // FATAL 级别
        FATAL = 5
    };

    /**
     *  将日志级别转成文本输出
     *	level 日志级别
     */
    static const char* ToString(LogLevel::Level level);
    
    /**
     *  将文本转换成日志级别
     *	str 日志级别文本
     */
    static LogLevel::Level FromString(const std::string& str);
};

/**
 *  日志事件
 */
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    /**
     *  构造函数
     *  logger 日志器
     *  level 日志级别
     *  file 文件名
     *  line 文件行号
     *  elapse 程序启动依赖的耗时(毫秒)
     *  thread_id 线程id
     *  p_id 进程id
     *  time 日志事件(秒)
     *  thread_name 线程名称
     */
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
            ,const char* file, int32_t line, uint32_t elapse
            ,uint32_t thread_id, uint32_t p_id, uint64_t time
            ,const std::string& thread_name);

    /**
     *  返回文件名
     */
    const char* getFile() const { return m_file;}

    /**
     *  返回行号
     */
    int32_t getLine() const { return m_line;}

    /**
     *  返回耗时
     */
    uint32_t getElapse() const { return m_elapse;}

    /**
     *  返回线程ID
     */
    uint32_t getThreadId() const { return m_threadId;}

    /**
     *  返回进程ID
     */
    uint32_t getPId() const { return m_pId;}

    /**
     *  返回时间
     */
    uint64_t getTime() const { return m_time;}

    /**
     *  返回线程名称
     */
    const std::string& getThreadName() const { return m_threadName;}

    /**
     *  返回日志内容
     */
    std::string getContent() const { return m_ss.str();}

    /**
     *  返回日志器
     */
    std::shared_ptr<Logger> getLogger() const { return m_logger;}

    /**
     *  返回日志级别
     */
    LogLevel::Level getLevel() const { return m_level;}

    /**
     *  返回日志内容字符串流
     */
    std::stringstream& getSS() { return m_ss;}

    /**
     *  格式化写入日志内容
     */
    void format(const char* fmt, ...);

    /**
     *  格式化写入日志内容
     */
    void format(const char* fmt, va_list al);
private:
    // 文件名
    const char* m_file = nullptr;
    // 行号
    int32_t m_line = 0;
    // 程序启动开始到现在的毫秒数
    uint32_t m_elapse = 0;
    // 线程ID
    uint32_t m_threadId = 0;
    // 进程ID
    uint32_t m_pId = 0;
    // 时间戳
    uint64_t m_time = 0;
    // 线程名称
    std::string m_threadName;
    // 日志内容流
    std::stringstream m_ss;
    // 日志器
    std::shared_ptr<Logger> m_logger;
    // 日志等级
    LogLevel::Level m_level;
};

/**
 *  日志事件包装器
 */
class LogEventWrap {
public:

    /**
     *  构造函数
     *  e 日志事件
     */
    LogEventWrap(LogEvent::ptr e);

    /**
     *  析构函数
     */
    ~LogEventWrap();

    /**
     *  获取日志事件
     */
    LogEvent::ptr getEvent() const { return m_event;}

    /**
     *  获取日志内容流
     */
    std::stringstream& getSS();
private:
    /**
     *  日志事件
     */
    LogEvent::ptr m_event;
};

/**
 *  日志格式化
 */
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    /**
     *  构造函数
     *	pattern 格式模板
	 *
     *  %m 消息
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %l 行号
     *  %T 制表符
     *  %P 进程id
     *  %N 线程名称
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%P%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    LogFormatter(const std::string& pattern);

    /**
     *  返回格式化日志文本
     *  logger 日志器
     *  level 日志级别
     *  event 日志事件
     */
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:

    /**
     *  日志内容项格式化
     */
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        /**
         *  析构函数
         */
        virtual ~FormatItem() {}
        /**
         *  格式化日志到流
         *  os 日志输出流
         *  logger 日志器
         *  level 日志等级
         *  event 日志事件
         */
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };

    /**
     *  初始化,解析日志模板
     */
    void init();

    /**
     *  是否有错误
     */
    bool isError() const { return m_error;}

    /**
     *  返回日志模板
     */
    const std::string getPattern() const { return m_pattern;}
private:
    // 日志格式模板
    std::string m_pattern;
    // 日志格式解析后格式
    std::vector<FormatItem::ptr> m_items;
    // 是否有错误
    bool m_error = false;

};

/**
 *  日志输出目标
 */
class LogAppender {
friend class Logger;
public:
    typedef std::shared_ptr<LogAppender> ptr;

    /**
     *  析构函数
     */
    virtual ~LogAppender() {}

    /**
     *  写入日志
     *  logger 日志器
     *  level 日志级别
     *  event 日志事件
     */
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

    /**
     *  更改日志格式器
     */
    void setFormatter(LogFormatter::ptr val);

    /**
     *  获取日志格式器
     */
    LogFormatter::ptr getFormatter();

    /**
     *  获取日志级别
     */
    LogLevel::Level getLevel() const { return m_level;}

    /**
     *  设置日志级别
     */
    void setLevel(LogLevel::Level val) { m_level = val;}
protected:
    // 日志级别
    LogLevel::Level m_level = LogLevel::DEBUG;
    // 是否有自己的日志格式器
    bool m_hasFormatter = false;
    // Mutex
    MutexLock m_mutex;
    // 日志格式器
    LogFormatter::ptr m_formatter = nullptr;
};

/**
 *  日志器
 */
class Logger : public std::enable_shared_from_this<Logger> {
friend class LoggerManager;
public:
    typedef std::shared_ptr<Logger> ptr;

    /**
     *  构造函数
     *	name 日志器名称
     */
    Logger(const std::string& name = "root");

    /**
     *  写日志
     *  level 日志级别
     *  event 日志事件
     */
    void log(LogLevel::Level level, LogEvent::ptr event);

    /**
     *  写debug级别日志
     *  event 日志事件
     */
    void debug(LogEvent::ptr event);

    /**
     *  写info级别日志
     *  event 日志事件
     */
    void info(LogEvent::ptr event);

    /**
     *  写warn级别日志
     *  event 日志事件
     */
    void warn(LogEvent::ptr event);

    /**
     *  写error级别日志
     *  event 日志事件
     */
    void error(LogEvent::ptr event);

    /**
     *  写fatal级别日志
     *  event 日志事件
     */
    void fatal(LogEvent::ptr event);

    /**
     *  添加日志目标
     *  appender 日志目标
     */
    void addAppender(LogAppender::ptr appender);

    /**
     *  删除日志目标
     *  appender 日志目标
     */
    void delAppender(LogAppender::ptr appender);

    /**
     *  清空日志目标
     */
    void clearAppenders();

    /**
     *  返回日志级别
     */
    LogLevel::Level getLevel() const { return m_level;}

    /**
     *  设置日志级别
     */
    void setLevel(LogLevel::Level val) { m_level = val;}

    /**
     *  返回日志名称
     */
    const std::string& getName() const { return m_name;}

    /**
     *  设置日志格式器
     */
    void setFormatter(LogFormatter::ptr val);

    /**
     *  设置日志格式模板
     */
    void setFormatter(const std::string& val);

    /**
     *  获取日志格式器
     */
    LogFormatter::ptr getFormatter();
private:
    // 日志名称
    std::string m_name;
    // 日志级别
    LogLevel::Level m_level;
    // Mutex
    MutexLock m_mutex;
    // 日志目标集合
    std::list<LogAppender::ptr> m_appenders;
    // 日志格式器
    LogFormatter::ptr m_formatter;
    // 主日志器
    Logger::ptr m_root;
};

/**
 *  输出到控制台的Appender
 */
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
};

/**
 *  输出到文件的Appender
 */
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    bool reopen();
private:
    // 文件路径
    std::string m_filename;
    // 文件流
    std::ofstream m_filestream;
    // 上次重新打开时间
    uint64_t m_lastTime = 0;
};

//日志管理器
class LoggerManager{
public:
	LoggerManager();
	Logger::ptr getRoot();
	Logger::ptr getTraceLog();
	Logger::ptr getErrorLog();
private:
	Logger::ptr m_root;
	Logger::ptr m_trace;
	Logger::ptr m_error;
};

}

#endif
