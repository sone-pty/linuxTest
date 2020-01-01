#ifndef __LOG_H
#define __LOG_H

#include <string>
#include <memory>
#include <list>
#include <fstream>
#include <sstream>
#include <ostream>
#include <vector>

namespace sone
{

//日志事件
class LogEvent{
public:
	typedef std::shared_ptr<LogEvent> ptr;
	
	LogEvent();
private:
	const char* m_file = nullptr; //文件名
	int32_t m_line = 0;		      //行号
	uint32_t m_elapse = 0;		  //程序启动到现在的毫秒数
	uint32_t m_threadId = 0;	  //线程id
	uint32_t m_fiberId = 0;		  //协程id
	uint64_t m_time = 0;		  //时间戳
	std::string m_content;		  //具体写入内容
};

//日志级别
class LogLevel{
public:
	enum Level
	{
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};
};

//日志格式器
class LogFormatter{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	
	LogFormatter(const std::string& pattern):m_pattern(pattern){}
	std::string format(LogEvent::ptr event);
//内嵌类FormatterItem定义日志的单项格式
private:
	class FormatItem{
	public:
		typedef std::shared_ptr<FormatItem> ptr;

		virtual ~FormatItem(){}
		virtual void format(std::ostream& os, LogEvent::ptr event) = 0;
	};
	
	//解析格式模板
	void init();
private:
	std::string m_pattern;
	std::vector<FormatItem::ptr> m_items;	
};

//日志输出地
class LogAppender{
public:
	typedef std::shared_ptr<LogAppender> ptr;
	
	virtual ~LogAppender(){}
	virtual void log(LogLevel::Level level, LogEvent::ptr event);
	void setFormatter(LogFormatter::ptr val){this->m_formatter = val;}
	LogFormatter::ptr getFormatter() const {return m_formatter;}
protected:
	LogLevel::Level m_level;
	LogFormatter::ptr m_formatter;
};

//日志器
class Logger{
public:
	typedef std::shared_ptr<Logger> ptr;

	Logger(const std::string& name = "root");
	void log(LogLevel::Level level, LogEvent::ptr event);
	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);

	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);
	LogLevel::Level getLevel() const {return m_level;}
	void setLevel(LogLevel::Level level){this->m_level = level;}
private:
	//日志名称
	std::string m_name;
	//日志级别
	LogLevel::Level m_level;
	//输出目的地集合
	std::list<LogAppender::ptr> m_appenders;	
};

//输出到控制台的Appender
class ConsoleLogAppender : public LogAppender{
public:
	void log(LogLevel::Level level, LogEvent::ptr event) override;
};

//输出到文件的Appender
class FileLogAppender : public LogAppender{
public:
	FileLogAppender(const std::string& filename):m_filename(filename){}
	void log(LogLevel::Level level, LogEvent::ptr event) override;
private:
	std::string m_filename;
	std::ofstream m_filestream;
};

}

#endif
