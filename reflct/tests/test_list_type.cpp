#include "utils/type_list.hpp"
#include <iostream>
#include <type_traits>

// 定义测试结构体和类型
struct A
{
};
struct B
{
};
struct C
{
};

// 定义一个判断类型是否为整数的模板结构体
template <typename T>
struct is_integral
{
    static constexpr bool value = std::is_integral_v<T>;
};

// 定义一个将类型转换为指针的模板结构体
template <typename T>
struct to_pointer
{
    using type = T *;
};

int main()
{
    // 创建一个类型列表
    using MyList = cero::type_list<int, float, double, A, B, C>;

    // 测试类型列表的大小
    static_assert(cero::list_size_v<MyList> == 6, "Error: MyList size should be 6");

    // 测试获取列表中的元素类型
    static_assert(std::is_same_v<cero::list_element_t<MyList, 0>, int>, "Error: First element should be int");
    static_assert(std::is_same_v<cero::list_element_t<MyList, 1>, float>, "Error: Second element should be float");
    static_assert(std::is_same_v<cero::list_element_t<MyList, 2>, double>, "Error: Third element should be double");
    static_assert(std::is_same_v<cero::list_element_t<MyList, 3>, A>, "Error: Fourth element should be A");

    // 测试获取列表的头和尾
    static_assert(std::is_same_v<cero::list_head_t<MyList>, int>, "Error: Head element should be int");
    using TailList = cero::list_tail_t<MyList>;
    static_assert(cero::list_size_v<TailList> == 5, "Error: Tail list size should be 5");
    static_assert(std::is_same_v<cero::list_head_t<TailList>, float>, "Error: Tail list head should be float");

    // 测试添加类型到列表的开头
    using NewList = cero::list_add_to_first_t<MyList, char>;
    static_assert(cero::list_size_v<NewList> == 7, "Error: NewList size should be 7");
    static_assert(std::is_same_v<cero::list_head_t<NewList>, char>, "Error: NewList head should be char");

    // 测试将函数模板应用于列表元素
    using PointerList = cero::list_foreach_t<MyList, to_pointer>;
    static_assert(std::is_same_v<cero::list_element_t<PointerList, 0>, int *>, "Error: First element should be int*");
    static_assert(std::is_same_v<cero::list_element_t<PointerList, 1>, float *>, "Error: Second element should be float*");

    // 测试任意元素是否满足条件
    static_assert(cero::disjunction_v<MyList, is_integral>, "Error: At least one element should be integral");

    // 测试所有元素是否满足条件
    static_assert(!cero::conjunction_v<MyList, is_integral>, "Error: Not all elements should be integral");

    // 测试合并两个类型列表
    using AnotherList = cero::type_list<long, short>;
    using MergedList = cero::concat_t<MyList, AnotherList>;
    static_assert(cero::list_size_v<MergedList> == 8, "Error: MergedList size should be 8");

    // 测试类型列表和tuple的相互转换
    using MyTuple = cero::typelist_to_tuple_t<MyList>;
    static_assert(std::is_same_v<MyTuple, std::tuple<int, float, double, A, B, C>>, "Error: MyTuple should match");

    using MyNewTypeList = cero::tuple_to_typelist<MyTuple>;
    static_assert(std::is_same_v<MyNewTypeList, MyList>, "Error: MyNewTypeList should match MyList");

    // 测试过滤类型列表
    using FilteredList = cero::list_filter_t<MyList, is_integral>;
    static_assert(cero::list_size_v<FilteredList> == 1, "Error: FilteredList size should be 1");
    static_assert(std::is_same_v<cero::list_head_t<FilteredList>, int>, "Error: FilteredList head should be int");

    std::cout << "All static assertions passed." << std::endl;
    return 0;
}
