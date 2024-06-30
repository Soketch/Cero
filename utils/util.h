#include <unistd.h>
#include <experimental/filesystem> //c++17文件系统
#include <string>

namespace fs = std::experimental::filesystem;

namespace cero
{
    // 返回当前路径
    std::string GetCurPath();

    // 返回当前日期时间
    const std::string GetCurrentdatetime();
    // 返回当前日期时间
    const std::string GetCurrentDateTime();

    // 获取当前进程id
    pid_t GetProcessId();
    // 获取当前线程id
    uint32_t GetThreadId();

    // 获取当前协程id
    uint32_t GetFiberId();

}