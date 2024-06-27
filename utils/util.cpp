#include "util.h"
#include <ctime>
#include <iomanip>
#include <sys/syscall.h>

namespace cero
{
    // 当前路径返回
    std::string getCurPath()
    {
        fs::path curPath = fs::current_path();
        return curPath;
    }

    // 返回当前日期时间
    const std::string getCurrentdatetime()
    {
        const std::string format = "%Y-%m-%d %H:%M:%S";
        std::time_t t = std::time(nullptr); // 获取当前时间戳
        struct tm tm;
        localtime_r(&t, &tm); // 转换为本地时间结构 tm

        char tm_buf[64];                                       // 大小适当，用于存储格式化后的时间字符串
        strftime(tm_buf, sizeof(tm_buf), format.c_str(), &tm); // 格式化时间到 tm_buf

        return std::string(tm_buf); // 返回格式化后的时间字符串
    }

    /**
     * @brief 获取当前本地日期时间的字符串表示
     * @return 返回格式为 "%Y-%m-%d %H:%M:%S" 的当前日期时间字符串
     */
    const std::string getCurrentDateTime()
    {
        auto now = std::chrono::system_clock::now();                   // 获取当前时间点
        std::time_t now_c = std::chrono::system_clock::to_time_t(now); // 转换为time_t类型

        std::ostringstream ss;                                            // 创建输出字符串流
        ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S"); // 格式化时间并写入字符串流
        return ss.str();                                                  // 返回格式化后的时间字符串
    }

    uint32_t getThreadId()
    {

        return syscall(SYS_gettid);
    }

    pid_t getProcessId()
    {
        return getpid();
    }
}