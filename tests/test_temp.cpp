#include <iostream>
#include <string>
#include <sstream>
#include <stdarg.h>
#include "utils/util.h"

std::string format(const char *fmt, ...)
{
    std::stringstream ss; // 创建一个字符串流，用于构建格式化后的字符串。
    va_list args;         // 定义一个变量列表类型，用于存储不定数量的参数。
    va_start(args, fmt);  // 初始化变量列表，`fmt` 是最后一个确定的参数。

    char *buf = nullptr;                  // 定义一个字符指针，用于存储格式化后的字符串。
    int len = vasprintf(&buf, fmt, args); // 使用 `vasprintf` 将格式化后的字符串存储到 `buf` 中，返回格式化后的字符串长度。

    if (len != -1) // 检查 `vasprintf` 是否成功。如果成功，则 `len` 不为 -1。
    {
        ss << std::string(buf, len); // 将格式化后的字符串添加到字符串流中。
        free(buf);                   // 释放由 `vasprintf` 分配的内存。
    }

    va_end(args); // 结束变量列表处理。

    return ss.str(); // 返回格式化后的字符串。
}

int main()
{
    // 使用format函数格式化字符串
    // std::string formattedString = format("Hello, %s! You have %d new messages.", "Alice", 5);

    // 输出格式化后的字符串
    // std::cout << formattedString << std::endl;

    std::cout << "当前日期：" << cero::getCurrentdatetime() << std::endl;

    return 0;
}
