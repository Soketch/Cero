#include "log.h"
#include <vector>
namespace cero
{

    const char *LogLevel::ToString(LogLevel::Level level)
    {
        switch (level)
        {
#define XX(name)         \
    case LogLevel::name: \
        return #name;    \
        break;

            XX(DEBUG);
            XX(INFO);
            XX(WARN);
            XX(ERROR);
            XX(FATAL);
#undef XX
        default:
            return "UNKNOWN";
            break;
        }
        return "UNKNOWN";
    }

    LogLevel::Level LogLevel::FromString(const std::string &str)
    {
#define XX(level, v)            \
    if (str == #v)              \
    {                           \
        return LogLevel::level; \
    }
        XX(DEBUG, debug);
        XX(INFO, info);
        XX(WARN, warn);
        XX(ERROR, error);
        XX(FATAL, fatal);

        XX(DEBUG, DEBUG);
        XX(INFO, INFO);
        XX(WARN, WARN);
        XX(ERROR, ERROR);
        XX(FATAL, FATAL);
        return LogLevel::UNKNOWN;
#undef XX
    }

    LogEvent::LogEvent(LogLevel::Level level, const char *file, int32_t line, uint32_t elapse, uint32_t thread_id, uint32_t fiber_id, uint64_t time)
        : m_level(level), m_file(file), m_line(line), m_elapse(elapse), m_threadId(thread_id), m_fiberId(fiber_id), m_time(time)
    {
    }

    Logger::Logger(const std::string &name)
        : m_name(name),
          m_level(LogLevel::DEBUG) // 设置日志器默认级别DEBUG
    {
    }
    void Logger::log(LogEvent::ptr event)
    {
        if (event->getLevel() >= m_level)
        {
            for (auto &i : m_appenders)
            {
                i->log(event);
            }
        }
    }

    void Logger::addAppender(LogAppender::ptr appender)
    {
        m_appenders.push_back(appender);
    }

    void Logger::delAppender(LogAppender::ptr appender)
    {
        for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it)
        {
            if (*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
        }
    }

    void StdoutLogAppender::log(LogEvent::ptr event)
    {
        // 格式化时间
        const std::string format = "%Y-%m-%d %H:%M:%S";
        struct tm tm;
        time_t t = event->getTime();
        localtime_r(&t, &tm);
        char tm_buf[64];
        strftime(tm_buf, sizeof(tm_buf), format.c_str(), &tm);

        std::cout
            //<< event->getTime() << " "
            << tm_buf << "    "
            << event->getThreadId() << " "
            << event->getFiberId() << " "
            << "["
            << LogLevel::ToString(event->getLevel())
            << "] "
            << event->getFile() << ":" << event->getLine() << " "
            << "输出到控制台的信息"
            << std::endl;
    }

    FileLogAppender::FileLogAppender(const std::string &filename)
        : m_filename(filename)
    {
    }

    void FileLogAppender::log(LogEvent::ptr event)
    {
        std::cout << "输出到文件：" << m_filename << std::endl;
    }

    LogFormatter::LogFormatter(const std::string &pattern)
        : m_pattern(pattern)
    {
        init(); // 在初始化的时候就把pattern解析好
    }

    void LogFormatter::init()
    {
        // 定义一个存储解析结果的向量
        // 元素为三元组（符号, 子串, 类型），类型0表示普通字符串，类型1表示可解析字符串
        std::vector<std::tuple<std::string, std::string, int>> vec;

        // 用于存储普通字符串的临时变量
        std::string nstr;

        // 遍历日志格式字符串 m_pattern
        for (size_t i = 0; i < m_pattern.size(); ++i)
        {
            // 如果不是 '%' 号，直接添加到 nstr 中
            if (m_pattern[i] != '%')
            {
                nstr.append(1, m_pattern[i]);
                continue;
            }

            // 如果遇到 '%'
            if ((i + 1) < m_pattern.size())
            {
                // 如果是 '%%'，将 '%' 作为普通字符处理
                if (m_pattern[i + 1] == '%')
                {
                    nstr.append(1, '%');
                    continue;
                }
            }

            // 开始解析 '%' 后的字符
            size_t n = i + 1;
            int fmt_status = 0;   // 标识是否进入大括号内的状态
            size_t fmt_begin = 0; // 记录大括号开始的位置

            std::string str; // 存储符号
            std::string fmt; // 存储大括号内的格式

            // 解析 '%' 后的字符，直到遇到不是字母、'{' 或 '}' 的字符
            while (n < m_pattern.size())
            {
                if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{' && m_pattern[n] != '}'))
                {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    break;
                }

                if (fmt_status == 0)
                {
                    if (m_pattern[n] == '{')
                    {
                        // 遇到 '{'，表示即将进入格式内容部分
                        str = m_pattern.substr(i + 1, n - i - 1);
                        fmt_status = 1; // 标记进入 '{'
                        fmt_begin = n;  // 记录 '{' 的位置
                        ++n;
                        continue;
                    }
                }
                else if (fmt_status == 1)
                {
                    if (m_pattern[n] == '}')
                    {
                        // 遇到 '}'，表示格式内容结束
                        fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                        fmt_status = 0;
                        ++n;
                        break;
                    }
                }
                ++n;
                // 处理到字符串末尾的情况
                if (n == m_pattern.size())
                {
                    if (str.empty())
                    {
                        str = m_pattern.substr(i + 1);
                    }
                }
            }

            // 处理结束后，将解析的内容存储到 vec 中
            if (fmt_status == 0)
            {
                if (!nstr.empty())
                {
                    // 保存普通字符
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    nstr.clear();
                }
                // 保存可解析字符串
                vec.push_back(std::make_tuple(str, fmt, 1));
                i = n - 1;
            }
            else if (fmt_status == 1)
            {
                // 格式错误，未找到匹配的 '}'
                std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
                vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
            }
        }

        // 保存剩余的普通字符串
        if (!nstr.empty())
        {
            vec.push_back(std::make_tuple(nstr, "", 0));
        }

        // 输出解析结果
        for (auto &it : vec)
        {
            std::cout
                << std::get<0>(it)
                << " : " << std::get<1>(it)
                << " : " << std::get<2>(it)
                << std::endl;
        }
    }

    std::string LogFormatter::format()
    {
        return std::string();
    }
}