#include "src/log.h"
#include "utils/util.h"
using namespace cero;

int main(int argc, char **argv)
{
    // 创建一个日志事件(这里的内容随便定义，因为我们没有真正用到它)
    LogEvent::ptr event(new LogEvent(
        LogLevel::INFO,
        "log.txt",
        1,
        1234567,
        2,
        3,
        time(0)));
    // 元素为三元组（符号, 子串, 类型），类型0表示普通字符串，类型1表示可解析字符串
    LogFormatter::ptr formatter(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));

    formatter->format(event);

    return 0;
}
