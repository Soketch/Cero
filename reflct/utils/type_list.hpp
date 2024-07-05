#pragma once
#include <tuple>
#include <cstddef>

namespace cero
{
    /**
     * @brief 通过类型容器（List）来存储类型 =>[ type Container ==>(store) type ]
     * @tparam Ts 
     */
    template<typename... Ts>
    struct type_list{
        using self_type = type_list<Ts...>;
        static constexpr size_t size = sizeof...(Ts);
    };

    namespace detail
    {
        // 容器元素
        template <typename, size_t>
        struct list_element;

        template<template <typename...> typename ListType, typename T, typename... Ts, size_t N>
        struct list_element<ListType<T, Ts...>, N>
            : list_element<ListType<Ts...>, N-1>
        {};

        template <template <typename...> typename ListType, typename T, typename... Ts>
        struct list_element<ListType<T, Ts...>, 0>
        {
            using type = T;
        };
    
    
        template <typename>
        struct list_size;

        template <template <typename...> typename ListType, typename... Ts>
        struct list_size<ListType<Ts...>>
        {
            static constexpr size_t value = sizeof...(Ts);
        };

        template <typename>
        struct list_head;

        template <template <typename...> typename ListType, typename T, typename... Ts>
        struct list_head<ListType<T, Ts...>>
        {
            using type = T;
        };

        template <typename>
        struct list_tail;

        template <template <typename...> typename ListType, typename T, typename... Ts>
        struct list_tail<ListType<T, Ts...>>
        {
            using type = ListType<Ts...>;
        };

        template <typename List, typename T>
        struct list_add_to_first;

        template <template <typename...> typename ListType, typename... Ts, typename T>
        struct list_add_to_first<ListType<Ts...>, T>
        {
            using type = ListType<T, Ts...>;
        };

    } // namespace detail

    /**
     * @brief  获取类型列表中的第n个元素(从类型列表或std::tuple或其他)
     * @tparam List
     * @tparam N 元素索引
     */
    template <typename List, size_t N>
    using list_element_t = typename detail::list_element<List, N>::type;

    /**
     * @brief 获取类型列表的长度
     * @tparam List 
     */
    template <typename List>
    constexpr size_t list_size_v = detail::list_size<List>::value;

    /**
     * @brief 判断列表是否为空
     * @tparam List 
     */
    template <typename List>
    constexpr bool is_list_empty_v = list_size_v<List> == 0;

    /**
     * @brief 获取列表中第一个元素类型
     * @tparam List 
     */
    template <typename List>
    using list_head_t = typename detail::list_head<List>::type;

    /**
     * @brief 获取列表最后一个元素类型
     * @tparam List
     */
    template <typename List>
    using list_tail_t = typename detail::list_tail<List>::type;

    template <typename List, typename T>
    using list_add_to_first_t = typename detail::list_add_to_first<List, T>::type;

    namespace detail{

        template <typename List, size_t N, template <typename> typename F>
        struct apply_to_element
        {
            using type = F<list_element_t<List, N>>;
        };
    } //namespace detail

    /**
     * @brief 将模板函数结构体应用于列表元素
     *
     * @tparam List
     * @tparam N
     * @tparam F  如同 => `template<typename> struct { constexpr auto value = XXX };`,
     * 接收一个类型并给出一个值
     */
    template <typename List, size_t N, template <typename> typename F>
    using apply_to_element_t = typename detail::apply_to_element<List, N, F>::type;

    namespace detail{
        template <typename List, template<typename> typename F>
        struct list_foreach{
        };

        template <template <typename...> typename ListType, template <typename> typename F, typename... Ts>
        struct list_foreach<ListType<Ts...>, F>
        {
            using type = ListType<typename F<Ts>::type...>;
        };

    } // namespace detai

    /**
     * @brief 遍历list获取类型
     * @tparam List 
     * @tparam F 
     */
    template <typename List, template <typename> typename F>
    using list_foreach_t = typename detail::list_foreach<List, F>::type;

    namespace detail
    {
        //提取/提纯操作
        template <typename List, template<typename> typename F>
        struct disjunction{
            static constexpr bool value = F < list_head_t<List>>::value || disjunction<list_tail_t<List>, F>::value;
        };

        template <template <typename...> typename ListType,
                  template <typename> typename F>
        struct disjunction<ListType<>, F>
        {
            static constexpr bool value = false;
        };

        // 结合/凝合
        template <typename List, template <typename> typename F>
        struct conjunction
        {
            static constexpr bool value =
                F<list_head_t<List>>::value &&
                disjunction<list_tail_t<List>, F>::value;
        };

        template <template <typename...> typename ListType,
                  template <typename> typename F>
        struct conjunction<ListType<>, F>
        {
            static constexpr bool value = true;
        };

        // 合并两个list
        template <typename List1, typename List2>
        struct concat;

        template <template <typename...> typename ListType, typename... Ts1,
                  typename... Ts2>
        struct concat<ListType<Ts1...>, ListType<Ts2...>>
        {
            using type = ListType<Ts1..., Ts2...>;
        };

        // tylist 转换为 tuple
        template <typename TypeList>
        struct typelist_to_tuple;

        template <typename... Ts>
        struct typelist_to_tuple<type_list<Ts...>>
        {
            using type = std::tuple<Ts...>;
        };

        // tuple 转换为 typelist
        template <typename Tuple>
        struct tuple_to_typelist;

        template <typename... Ts>
        struct tuple_to_typelist<std::tuple<Ts...>>
        {
            using type = type_list<Ts...>;
        };

        // 过滤list
        template <typename List, template <typename> typename F>
        struct list_filter;

        template <template <typename...> typename ListType, typename T, typename... Ts,
                  template <typename> typename F>
        struct list_filter<ListType<T, Ts...>, F>
        {
            using type = std::conditional_t<
                F<T>::value,
                list_add_to_first_t<typename list_filter<ListType<Ts...>, F>::type, T>,
                typename list_filter<ListType<Ts...>, F>::type>;
        };

        template <template <typename...> typename ListType,
                  template <typename> typename F>
        struct list_filter<ListType<>, F>
        {
            using type = ListType<>;
        };

    } //namespace detail

    /**
     * @brief 利用disjunction提取 使用F（function struct F）检查任意一个元素是否为真
     * @tparam List
     * @tparam F 接收类型并给出constexpr静态bool值;
     */
    template <typename List, template <typename> typename F>
    constexpr bool disjunction_v = detail::disjunction<List, F>::value;

    /**
     * @brief 利用conjunction结合，使用函数结构体F检查所有元素是否为真
     * @tparam List
     * @tparam F 接收类型并给出constexpr静态bool值;
     */
    template <typename List, template <typename> typename F>
    constexpr bool conjunction_v = detail::conjunction<List, F>::value;

    /**
     * @brief 合并 两个type list
     * @tparam List1 
     * @tparam List2 
     */
    template <typename List1, typename List2>
    using concat_t = typename detail::concat<List1, List2>::type;

    template <typename TypeList>
    using typelist_to_tuple_t = typename detail::typelist_to_tuple<TypeList>::type;

    template <typename Tuple>
    using tuple_to_typelist = typename detail::tuple_to_typelist<Tuple>::type;

    template <typename List, template <typename> typename F>
    using list_filter_t = typename detail::list_filter<List, F>::type;
}