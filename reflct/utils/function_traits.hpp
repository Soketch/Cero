#pragma once
#include "type_list.hpp"

namespace cero
{
    namespace detail{

        template <typename F>
        struct function_type;

        template <typename Ret, typename... Args>
        struct function_type<Ret (*)(Args...)>
        {
            using type = Ret(Args...);
        };

        template <typename Ret, typename Class, typename... Args>
        struct function_type<Ret (Class::*)(Args...)>
        {
            using type = Ret (Class::*)(Args...);
        };

        template <typename Ret, typename Class, typename... Args>
        struct function_type<Ret (Class::*)(Args...) const>
        {
            using type = Ret (Class::*)(Args...) const;
        };
    }//namespace detail

    /**
     * @brief 通过函数指针获取函数类型
     * @tparam F 
     */
    template <typename F>
    using function_type_t = typename detail::function_type<F>::type;


    namespace detail{

        template <typename Ret, typename... Args>
        auto function_pointer_to_type(int, Ret (*)(Args...)) -> Ret (*)(Args...);

        template <typename Ret, typename Class, typename... Args>
        auto function_pointer_to_type(char, Ret (Class::*)(Args...))
            -> Ret (Class::*)(Args...);

        template <typename Ret, typename Class, typename... Args>
        auto function_pointer_to_type(char, Ret (Class::*)(Args...) const)
            -> Ret (Class::*)(Args...) const;

    }   //namespace detail
    
    /**
     * @brief 通过函数指针 获取 函数指针类型
     * @tparam F 
     */
    template <auto F>
    using function_pointer_type_t =
        decltype(detail::function_pointer_to_type(0, F));
    // template <auto F>
        // using function_pointer_type_t = typename detail::function_pointer_to_type(0, F);
    
    /**
     * @brief 通过函数指针 获取函数类型
     * @tparam F 
     */
    template <auto F>
    using function_type_from_pointer_t =
        function_type_t<decltype(detail::function_pointer_to_type(0, F))>;
  
    namespace detail{
        // 萃取函数信息
        template <typename Func>
        struct basic_function_traits;

        template <typename Ret, typename... Args>
        struct basic_function_traits<Ret(Args...)>
        {
            using args = type_list<Args...>;
            using return_type = Ret;
        };
    }//namespace detail

    // 函数萃取
    template <typename Func>
    struct function_traits;

    template <typename Ret, typename... Args>
    struct function_traits<Ret(Args...)>
        : detail::basic_function_traits<Ret(Args...)>
    {
        using type = Ret(Args...);
        using args_with_class = type_list<Args...>;
        using pointer = Ret (*)(Args...);
        static constexpr bool is_member = false;
        static constexpr bool is_const = false;
    };

    template <typename Ret, typename Class, typename... Args>
    struct function_traits<Ret (Class::*)(Args...)>
        : detail::basic_function_traits<Ret(Args...)>
    {
        using type = Ret (Class::*)(Args...);
        using args_with_class = type_list<Class *, Args...>;
        using pointer = Ret (Class::*)(Args...);
        static constexpr bool is_member = true;
        static constexpr bool is_const = false;
    };

    template <typename Ret, typename Class, typename... Args>
    struct function_traits<Ret (Class::*)(Args...) const>
        : detail::basic_function_traits<Ret(Args...)>
    {
        using type = Ret (Class::*)(Args...) const;
        using args_with_class = type_list<Class *, Args...>;
        using pointer = Ret (Class::*)(Args...) const;
        static constexpr bool is_member = true;
        static constexpr bool is_const = true;
    };

    namespace detail{

        // 函数指针萃取
        template <auto F>
        struct function_pointer_traits
            : function_traits<function_type_from_pointer_t<F>>
        {
        };
    }//namespace detail

    /**
     * @brief 萃取函数指针，提取出函数信息
     * @tparam F 
     */
    template <auto F>
    using function_pointer_traits = detail::function_pointer_traits<F>;

    /**
     * @brief 检查类型是否是一个函数指针或者函数（Function or Function pointer）
     * @tparam T 
     */
    template <typename T>
    constexpr bool is_function_v = std::is_function_v<T> || std::is_member_function_pointer_v<T>;
} //namespace cero

