#include "src/log.h"
#include "utils/util.h"
using namespace cero;

int main(int argc, char **argv)
{
    // 创建一个日志事件(这里的内容随便定义，因为我们没有真正用到它)
    LogEvent::ptr event(new LogEvent(
        LogLevel::INFO, // 日志级别
        __FILE__,       // 文件名称
        __LINE__,       // 行号
        1234567,        // 运行时间
        getThreadId(),  // 线程ID
        0,              // 协程ID
        time(0)         // 当前时间
        ));

    Logger::ptr lg(new Logger("XYZ"));
    // 添加控制台输出适配器
    StdoutLogAppender::ptr stdApd(new StdoutLogAppender());
    lg->addAppender(stdApd);
    // 输出
    lg->log(event);
    return 0;
}
