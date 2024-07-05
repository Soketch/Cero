#pragma once
#include <iostream>
#include <type_traits>
namespace cero
{

    namespace detail
    {

        template <typename T>
        struct variable_type
        {
            using type = T;
        };

        template <typename Class, typename T>
        struct variable_type<T Class::*>
        {
            using type = T;
        };

    } // namespace detail

    /**
     * @brief 获取变量类型
     * 普通变量-类变量 => T
     * @tparam T 变量类型
     */
    template <typename T>
    using variable_type_t = typename detail::variable_type<T>::type;

    namespace detail
    {
        template <typename T>
        auto variable_pointer_to_type(long, T *) -> T;

        template <typename Class, typename T>
        auto variable_pointer_to_type(char, T Class::*) -> T;
    }  //namespace detail

    /**
     * @brief 通过变量的指针获取变量类型
     * @tparam V 
     */
    template <auto V>
    using variable_pointer_to_type_t = decltype(detail::variable_pointer_to_type(0, V));


    // 内置部分
    namespace internal{
        template<typename T>
        struct basic_variable_traits{
            using type = variable_type_t<T>;
            static constexpr bool is_member = std::is_member_pointer_v<T>;
        };
    } // namespace internal


    // 萃取
    /**
     * @brief 从变量类型中提取变量信息
     * @tparam T => Func
     */
    template <typename T>
    struct variable_traits;

    template<typename T>
    struct variable_traits<T*>
        : internal::basic_variable_traits<T>{
        using pointer = T *;
    };

    template<typename Class, typename T>
    struct variable_traits<T Class::*>
        : internal::basic_variable_traits<T Class::*>{
        using pointer = T *;
        using clazz = Class;
    };
    

    namespace detail{

        template<auto V>
        struct variable_pointer_traits
            :variable_traits<decltype(V)>{
        };
    } // namespace detail


    /**
     * @brief 从变量指针中提取变量信息
     * @tparam V  => F
     */
    template <auto V>
    using variable_pointer_traits = detail::variable_pointer_traits<V>;

} //namespace cero