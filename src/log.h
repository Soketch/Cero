/**
 * @file log.h
 * @author {soketch} ({skgfwb@gmail.com})
 * @brief 日志类封装
 * @version 0.1
 * @date 2024-06-26
 */

#ifndef __LOG_H_
#define __LOG_H_
#include <iostream>
#include <memory>
#include <string>
#include <list>

namespace cero
{
    class Logger;

    // 日志等级（级别）类
    class LogLevel
    {
    public:
        enum Level
        {
            UNKNOWN = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
        static const char *ToString(LogLevel::Level level);
        LogLevel::Level FromString(const std::string &str);
    };
    // 日志事件类
    class LogEvent
    {
    public:
        using ptr = std::shared_ptr<LogEvent>;

        LogEvent(LogLevel::Level level, const char *file,
                 int32_t line, uint32_t elapse,
                 uint32_t thread_id, uint32_t fiber_id,
                 uint64_t time);

        const char *getFile() const { return m_file; }
        int32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint64_t getTime() const { return m_time; }
        LogLevel::Level getLevel() const { return m_level; }

    private:
        LogLevel::Level m_level;      // 日志级别
        const char *m_file = nullptr; // 文件号
        int32_t m_line = 0;           // 行号
        uint32_t m_elapse = 0;        // 程序启动到现在的毫秒数
        uint32_t m_threadId = 0;      // 线程号
        uint32_t m_fiberId = 0;       // 协程号
        uint64_t m_time = 0;          // 时间戳
    };

    // 日志输出地类
    class LogAppender
    {
    public:
        using ptr = std::shared_ptr<LogAppender>;

        virtual ~LogAppender() {}
        virtual void log(LogEvent::ptr event) = 0; // 纯虚函数 -- 实现各个子类不一样，由各个子类自己决定

    private:
    };

    // 日志格式器类
    class LogFormatter
    {
    public:
        using ptr = std::shared_ptr<LogFormatter>;
        /**
         * @brief 构造函数
         * @param[in] pattern 格式模板
         * @details
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
         *  %F 协程id
         *  %N 线程名称
         *
         *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
         */
        LogFormatter(const std::string &pattern);

        /**
         * @brief 初始化,解析日志模板
         */
        void init();

        /**
         * @brief 返回格式化日志文本
         * @param[in] logger 日志器
         * @param[in] level 日志级别
         * @param[in] event 日志事件
         */
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

        // format嵌套类
        class FormatItem
        {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            // 有子类 需要虚析构
            virtual ~FormatItem() {}
            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

    private:
        /// 日志格式模板
        std::string m_pattern;

        std::vector<FormatItem::ptr> m_items;
    };

    // 日志器类
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;

        // 构造函数 => 默认name:root
        Logger(const std::string &name = "root");

        const std::string getName() const { return m_name; }
        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(const LogLevel::Level v) { m_level = v; }

        // 输出日志
        void log(LogEvent::ptr event);
        // 新增一个适配器
        void addAppender(LogAppender::ptr appender);
        // 删除一个适配器
        void delAppender(LogAppender::ptr appender);

    private:
        std::string m_name;
        // 日志器最大输出入职级别
        LogLevel::Level m_level;

        // Appender集合
        std::list<LogAppender::ptr> m_appenders;
    };

    // 输出到控制台日志类
    class StdoutLogAppender : public LogAppender
    {
    public:
        using ptr = std::shared_ptr<StdoutLogAppender>;
        void log(LogEvent::ptr event) override;
    };

    // 输出到文件日志类
    class FileLogAppender : public LogAppender
    {
    public:
        using ptr = std::shared_ptr<FileLogAppender>;

        FileLogAppender(const std::string &filename);
        void log(LogEvent::ptr event) override;

    private:
        // 日志输出的目的文件
        std::string m_filename;
    };
}

#endif