#include <unistd.h>
#include <experimental/filesystem> //c++17文件系统
#include <string>

namespace fs = std::experimental::filesystem;

namespace cero
{
    // 返回当前路径
    std::string getCurPath();

    // 返回当前日期时间
    const std::string getCurrentdatetime();
    // 返回当前日期时间
    const std::string getCurrentDateTime();

    // 获取当前进程id
    pid_t getProcessId();
    // 获取当前线程id
    uint32_t getThreadId();

}