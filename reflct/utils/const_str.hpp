#pragma once
#include <string_view>
#include <utility>

namespace cero
{

    /**
     * @brief 常量字符串模板，用于存储编译时确定的字符串
     * @tparam Char 字符类型，如 char 或 wchar_t
     * @tparam Chars 字符序列，用于存储字符串的每个字符
     */
    template <typename Char, Char... Chars>
    struct const_string
    {
        static constexpr Char data[] = {Chars..., 0}; // 静态常量字符数组，用于存储字符串内容，末尾自动添加 '\0' 终止符
        static constexpr size_t length = sizeof...(Chars); // 字符串长度，使用 sizeof...(Chars) 计算字符数量
        constexpr std::string_view str() const noexcept { return data; }// 返回字符串视图（string_view），用于只读访问字符串内容
    };

    /**
     * @brief 准备模板函数，将传入的类型 T 和索引序列转换为 const_string 实例
     * @tparam T 传入的类型，通常是一个带有静态 get 方法的结构体
     * @tparam N 索引序列，用于访问字符串的每个字符
     * @param t 传入的对象（未使用）
     * @return constexpr decltype(auto) 返回一个 const_string 实例
     */
    template <typename T, std::size_t... N>
    constexpr decltype(auto) prepareTmpl(T, std::index_sequence<N...>)
    {
        return const_string<char, T::get()[N]...>();
    }

    /**
     * @brief 准备函数，生成索引序列并调用 prepareTmpl
     * @tparam T 传入的类型，通常是一个带有静态 get 方法的结构体
     * @param t 传入的对象（未使用）
     * @return constexpr decltype(auto) 返回一个 const_string 实例
     */
    template <typename T>
    constexpr decltype(auto) prepare(T t)
    {
        // 使用 make_index_sequence 生成从 0 到 sizeof(T::get()) - 2 的索引序列
        return prepareTmpl(t, std::make_index_sequence<sizeof(T::get()) - 1>());
    }

    /**
     * @brief 宏定义，用于简化常量字符串的创建
     * 使用方式：CONST_STR("example")
     * 该宏会生成一个带有静态 get 方法的匿名结构体，并调用 prepare 函数生成 const_string 实例
     * @param s 字符串文字，用于创建 const_string 实例
     */
#define CONST_STR(s) \
    (::cero::prepare([] {                   \
        struct tmp {                                \
            static constexpr decltype(auto) get() { \
                return s;                           \
            }                                       \
        };                                          \
        return tmp{}; }()))

} // namespace cero



/* tag::案例

CONST_STR("HelloWorld")
-> ::cero::prepare(tmp{})
-> ::cero::prepareTmpl(tmp{}, std::make_index_sequence<10>())
-> const_string<char, 'H', 'e', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'>

*/