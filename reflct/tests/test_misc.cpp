#include <iostream>
#include <type_traits>
#include <vector>
#include <optional>
#include <string>
#include "utils/misc.hpp"

void test_misc_func_assert()
{
    // 测试 remove_cvref_t
    static_assert(std::is_same_v<cero::remove_cvref_t<const int &>, int>);
    static_assert(std::is_same_v<cero::remove_cvref_t<volatile int &&>, int>);

    // 测试 remove_all_pointers_t
    static_assert(std::is_same_v<cero::remove_all_pointers_t<int ***>, int>);

    // 测试 strip_type_t
    static_assert(std::is_same_v<cero::strip_type_t<const int *>, int>);
    static_assert(std::is_same_v<cero::strip_type_t<int[3]>, int>);

    // 测试 is_complex_type_v
    static_assert(cero::is_complex_type_v<const int *>);
    static_assert(!cero::is_complex_type_v<int>);

    // 测试 is_container_v
    static_assert(cero::is_container_v<std::vector<int>>);
    static_assert(!cero::is_container_v<int>);

    // 测试 is_std_array_v
    static_assert(cero::is_std_array_v<std::array<int, 3>>);
    static_assert(!cero::is_std_array_v<int>);

    // 测试 is_optional_v
    static_assert(cero::is_optional_v<std::optional<int>>);
    static_assert(!cero::is_optional_v<int>);

    // 测试 is_vector_v
    static_assert(cero::is_vector_v<std::vector<int>>);
    static_assert(!cero::is_vector_v<int>);

    // 测试 completely_strip_type_t
    static_assert(std::is_same_v<cero::completely_strip_type_t<const int *const *>, int>);

    // 测试 inner_type_t
    static_assert(std::is_same_v<cero::inner_type_t<std::vector<int>>, int>);
    static_assert(std::is_same_v<cero::inner_type_t<std::optional<int>>, int>);

    // 测试 pointer_layer_v
    static_assert(cero::pointer_layer_v<int *> == 1);
    static_assert(cero::pointer_layer_v<int **> == 2);
    static_assert(cero::pointer_layer_v<int ***> == 3);

    /* warn assert part
     */
    static_assert(std::is_same_v<cero::completely_strip_type_t<const int &>, int>);
    static_assert(std::is_same_v<cero::completely_strip_type_t<int[3]>, int>);

    std::cout << "All tests passed!" << std::endl;
}

int main()
{
    test_misc_func_assert();

    return 0;
}
