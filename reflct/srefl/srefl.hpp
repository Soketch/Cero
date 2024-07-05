#pragma once

#include "utils/function_traits.hpp"
#include "utils/type_list.hpp"
#include "utils/variable_traits.hpp"

#include <string_view>
#include <utility>
#include <functional>

namespace cero{

    namespace srefl{

        /**
         * @brief 附加到字段/函数的属性
         * @tparam Attrs
         */
        template <typename... Attrs>
        using attr_list = type_list<Attrs...>;

        namespace internal{
            
            // 字段萃取
            template <typename T, bool>
            struct basic_field_traits;

            // 字段萃取，函数萃取的偏特化
            template <typename T>
            struct basic_field_traits<T, true> : function_traits<T>
            {
                constexpr bool is_const_member() const noexcept
                {
                    return function_traits<T>::is_const;
                }

                constexpr bool is_member() const noexcept
                {
                    return function_traits<T>::is_member;
                }

                constexpr bool is_function() const noexcept
                {
                    return true;
                }

                constexpr bool is_variable() const noexcept
                {
                    return false;
                }
            };

            // 字段萃取 ， 变量萃取的偏特化
            template <typename T>
            struct basic_field_traits<T, false> : variable_traits<T>
            {
                constexpr bool is_const_member() const noexcept
                {
                    return false;
                }

                constexpr bool is_member() const noexcept
                {
                    return variable_traits<T>::is_member;
                }

                constexpr bool is_function() const noexcept
                {
                    return false;
                }

                constexpr bool is_variable() const noexcept
                {
                    return true;
                }
            };
        } // namespace internal

        /**
         * @brief 从 namespace/class prefix 中擦除 class/function/variable name  ，剥离为纯名称 to pure name
         *
         * @param name
         * @return constexpr std::string_view
         */
        inline constexpr std::string_view strip_name(std::string_view name)
        {
            std::string_view result = name;

            if (auto idx = name.find_last_of('&'); idx != std::string_view::npos)
            {
                name = name.substr(idx + 1, name.length());
            }
            if (auto idx = name.find_last_of(':'); idx != std::string_view::npos)
            {
                name = name.substr(idx + 1, name.length());
            }
            if (auto idx = name.find_first_of(')'); idx != std::string_view::npos)
            {
                name = name.substr(0, idx);
            }

            return name;
        }

        /**
         * @brief 提取类字段(成员变量，成员函数)信息
         * @tparam T 类型 type
         * @tparam Attrs 属性 attributes
         */
        template <typename T, typename... Attrs>
        struct field_traits : internal::basic_field_traits<T, is_function_v<T>>
        {
            constexpr field_traits(T &&pointer, std::string_view name,
                                   Attrs &&...attrs)
                : pointer_(std::forward<T>(pointer)),
                  name_(strip_name(name)),
                  attrs_(std::forward<Attrs>(attrs)...) {}

            /**
             * @brief 检查字段field是否是const成员(类const函数)
             */
            constexpr bool is_const_member() const noexcept
            {
                return base::is_const_member();
            }

            /**
             * @brief 检查字段是类成员或者是静态的static(局部) 或是 全局的global
             */
            constexpr bool is_member() const noexcept
            {
                return base::is_member();
            }

            /**
             * @brief 获取字段名
             */
            constexpr std::string_view name() const noexcept
            {
                return name_;
            }

            /**
             * @brief 获取指针
             */
            constexpr auto pointer() const noexcept
            {
                return pointer_;
            }

            /**
             * @brief 获取属性 attributes
             */
            constexpr auto &attrs() const noexcept
            {
                return attrs_;
            }

            /**
             * @brief 根据模板参数 T 的特性进行不同的调用操作
             * @tparam Args
             * @param args
             * @return decltype(auto)
             */
            template <typename... Args>
            decltype(auto) invoke(Args &&...args)
            {
                if constexpr (!is_function_v<T>)
                {
                    if constexpr (variable_traits<T>::is_member)
                    {
                        return std::invoke(this->pointer_, std::forward<Args>(args)...);
                    }
                    else
                    {
                        return *(this->pointer_);
                    }
                }
                else
                {
                    return std::invoke(this->pointer_, std::forward<Args>(args)...);
                }
            }

        private:
            using base = internal::basic_field_traits<T, is_function_v<T>>;

            T pointer_;  // 指针
            std::string_view name_;  // 字段名
            std::tuple<Attrs...> attrs_;  // 属性
        };

        /**
         * @brief 存储类的构造函数
         */
        template <typename... Args>
        struct ctor
        {
            using args = type_list<Args...>;
        };

        /**
         * @brief 存储基类
         */
        template <typename... Bases>
        struct base
        {
            using bases =  type_list<Bases...>;
        };

        template <typename T>
        struct base_type_info
        {
            using type = T;
            static constexpr bool is_final = std::is_final_v<T>;
        };

        template <typename T>
        struct enum_value
        {
            using value_type = T;
            // using underlying_type = std::underlying_type_t<T>;

            constexpr enum_value(value_type value, std::string_view name)
                : value{value}, name{name} {}

            T value;
            std::string_view name;
        };

        /**
         * @brief 存储类的类型信息
         *
         * @tparam T type
         * @tparam AttrList attributes
         */
        template <typename T>
        struct type_info;

    } // namespace srefl
} // namespace cero