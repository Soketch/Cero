/**
 * @file misc.hpp
 * @brief 模板和部分类型特征
 * 用于在编译时处理和分析各种类型，特别是容器、指针、限定符等类型
 * 移除类型限定符和指针 和 类型检测 和 获取指针层级与值
 * @version 0.1
 * @date 2024-07-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <array>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <list>

namespace cero
{

    // 用于移除类型 T 的引用(&和 &&) 和 const、volatile 限定符。
    template <typename T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    namespace detail
    {
        template <typename T, bool>
        struct remove_all_pointers;

        template <typename T>
        struct remove_all_pointers<T, false>
        {
            using type = T;
        };

        template <typename T>
        struct remove_all_pointers<T, true>
        {
            using type = typename remove_all_pointers<std::remove_pointer_t<T>,
                                                      std::is_pointer_v<T>>::type;
        };
    } // namespace detail

    /**
     * @brief 从type中移除所有指针
     */
    template <typename T>
    using remove_all_pointers_t =
        typename detail::remove_all_pointers<T, true>::type;

    /**
     * @brief 检查类型是否有限定符，比如(const, volatile)
     */
    template <typename T>
    constexpr bool has_qualifier_v = std::is_volatile_v<T> || std::is_const_v<T>;

    /**
     * @brief 检查类型是否有指针, 限定符qualifier, 引用reference or 数组array
     */
    template <typename T>
    constexpr bool is_complex_type_v = has_qualifier_v<T> || std::is_pointer_v<T> ||
                                       std::is_array_v<T> || std::is_reference_v<T>;

    /**
     * @brief 从类型中移除指针、限定符、引用、数组等
     *  remove qualifier/pointer/reference/array from type
     */
    template <typename T>
    using strip_type_t = remove_cvref_t<
        remove_all_pointers_t<std::decay_t<std::remove_all_extents_t<T>>>>;

    namespace detail
    {

        template <typename U>
        [[maybe_unused]] static auto is_container_test(int)
            -> decltype(std::declval<U>().begin(), std::declval<U>().end(),
                        std::true_type{});

        template <typename U>
        [[maybe_unused]] static std::false_type is_container_test(...);

        /**
         * @brief 检查类型是否为容器(具有begin()和end()成员函数)
         */
        template <typename T>
        struct is_container : decltype(detail::is_container_test<T>(0))
        {
        };

        // 类型擦除 ，完全擦除类型
        template <typename T, typename = void>
        struct completely_strip_type
        {
            using type = T;
        };

        template <typename T>
        struct completely_strip_type<T, std::enable_if_t<is_complex_type_v<T>>>
        {
            using type = typename completely_strip_type<strip_type_t<T>>::type;
        };

        // 检查是否是std::array
        template <typename T>
        struct is_std_array
        {
            static constexpr bool value = false;
        };

        template <typename T, size_t N>
        struct is_std_array<std::array<T, N>>
        {
            static constexpr bool value = true;
        };

        // 是否是 包装器 std::optional
        template <typename T>
        struct is_optional
        {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_optional<std::optional<T>>
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_vector
        {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_vector<std::vector<T>>
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_std_list
        {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_std_list<std::list<T>>
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_unordered_map
        {
            static constexpr bool value = false;
        };

        template <typename K, typename V>
        struct is_unordered_map<std::unordered_map<K, V>>
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_map
        {
            static constexpr bool value = false;
        };

        template <typename K, typename V>
        struct is_map<std::map<K, V>>
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_set
        {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_set<std::set<T>>
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_unordered_set
        {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_unordered_set<std::unordered_set<T>>
        {
            static constexpr bool value = true;
        };

        template <typename T>
        struct is_string
        {
            static constexpr bool value = std::is_same_v<T, std::string>;
        };

        // 内置类型
        template <typename T, typename = void>
        struct inner_type
        {
            using type = T;
        };

        template <typename T>
        struct inner_type<T, std::void_t<typename T::value_type>>
        {
            using type = typename T::value_type;
        };

        // 指针层级
        template <typename T>
        struct pointer_layer
        {
            constexpr static int value = 0;
        };

        template <typename T>
        struct pointer_layer<T *>
        {
            constexpr static int value = 1 + pointer_layer<T>::value;
        };

        template <typename T>
        struct array_element_type
        {
            using type = typename T::value_type;
        };

        template <typename T, size_t N>
        struct array_element_type<T[N]>
        {
            using type = T;
        };

    } // namespace detail

    /**
     * @brief 检查类型T的对象是否支持成员。begin()和。end()操作。
     */
    template <typename T>
    constexpr bool is_container_v = detail::is_container<T>::value;

    /**
     * @brief 从类型中移除 所有的指针、引用、限定符、数组
     * remove all qualifier/pointer/reference/array from type
     */
    template <typename T>
    using completely_strip_type_t = typename detail::completely_strip_type<T>::type;

    template <typename T>
    constexpr bool is_std_array_v = detail::is_std_array<T>::value;

    template <typename T>
    constexpr bool is_vector_v = detail::is_vector<T>::value;

    template <typename T>
    constexpr bool is_std_list_v = detail::is_std_list<T>::value;

    template <typename T>
    constexpr bool is_unordered_map_v = detail::is_unordered_map<T>::value;

    template <typename T>
    constexpr bool is_map_v = detail::is_map<T>::value;

    template <typename T>
    constexpr bool is_set_v = detail::is_set<T>::value;

    template <typename T>
    constexpr bool is_unordered_set_v = detail::is_unordered_set<T>::value;

    template <typename T>
    constexpr bool is_string_v = detail::is_string<T>::value;

    template <typename T>
    constexpr bool is_optional_v = detail::is_optional<T>::value;

    template <typename T>
    using array_element_t = typename detail::array_element_type<T>::type;

    /**
     * @brief 获取容器、包装器的内置类型
     * get container/std::optional inner type
     */
    template <typename T>
    using inner_type_t = typename detail::inner_type<T>::type;

    
    template <typename T>
    constexpr int pointer_layer_v =
        detail::pointer_layer<remove_cvref_t<T>>::value;

} // namespace cero