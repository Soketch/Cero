#include <iostream>
#include "utils/const_str.hpp"

int main()
{
    // 使用 CONST_STR 宏创建一个常量字符串实例
    constexpr auto my_string = CONST_STR("Hello, World!");

    // 打印字符串内容和长度
    std::cout << "String: " << my_string.str() << std::endl;
    std::cout << "Length: " << my_string.length << std::endl;

    // 检查字符串内容是否正确
    static_assert(my_string.str() == "Hello, World!", "String content is incorrect");
    static_assert(my_string.length == 13, "String length is incorrect");

    return 0;
}
