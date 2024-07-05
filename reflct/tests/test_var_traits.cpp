#include "utils/variable_traits.hpp"
#include <iostream>
#include <type_traits>

// 用于测试的结构体和变量
struct MyClass
{
    int intMember;
    float floatMember;
};

int globalInt;
float globalFloat;

int main()
{
    // 测试普通变量类型提取
    static_assert(std::is_same_v<cero::variable_type_t<int>, int>, "Error: variable_type_t<int> should be int");
    static_assert(std::is_same_v<cero::variable_type_t<float>, float>, "Error: variable_type_t<float> should be float");

    // 测试类成员变量类型提取
    static_assert(std::is_same_v<cero::variable_type_t<int MyClass::*>, int>, "Error: variable_type_t<int MyClass::*> should be int");
    static_assert(std::is_same_v<cero::variable_type_t<float MyClass::*>, float>, "Error: variable_type_t<float MyClass::*> should be float");

    // 测试通过指针获取变量类型
    static_assert(std::is_same_v<cero::variable_pointer_to_type_t<&globalInt>, int>, "Error: variable_pointer_to_type_t<&globalInt> should be int");
    static_assert(std::is_same_v<cero::variable_pointer_to_type_t<&globalFloat>, float>, "Error: variable_pointer_to_type_t<&globalFloat> should be float");

    // 测试通过类成员指针获取变量类型
    static_assert(std::is_same_v<cero::variable_pointer_to_type_t<&MyClass::intMember>, int>, "Error: variable_pointer_to_type_t<&MyClass::intMember> should be int");
    static_assert(std::is_same_v<cero::variable_pointer_to_type_t<&MyClass::floatMember>, float>, "Error: variable_pointer_to_type_t<&MyClass::floatMember> should be float");

    // 测试 variable_traits
    using intTraits = cero::variable_traits<int *>;
    static_assert(std::is_same_v<intTraits::type, int>, "Error: variable_traits<int*>::type should be int");
    static_assert(!intTraits::is_member, "Error: variable_traits<int*>::is_member should be false");

    using intMemberTraits = cero::variable_traits<int MyClass::*>;
    static_assert(std::is_same_v<intMemberTraits::type, int>, "Error: variable_traits<int MyClass::*>::type should be int");
    static_assert(intMemberTraits::is_member, "Error: variable_traits<int MyClass::*>::is_member should be true");
    static_assert(std::is_same_v<intMemberTraits::clazz, MyClass>, "Error: variable_traits<int MyClass::*>::clazz should be MyClass");

    // 测试 variable_pointer_traits
    using globalIntTraits = cero::variable_pointer_traits<&globalInt>;
    static_assert(std::is_same_v<globalIntTraits::type, int>, "Error: variable_pointer_traits<&globalInt>::type should be int");
    static_assert(!globalIntTraits::is_member, "Error: variable_pointer_traits<&globalInt>::is_member should be false");

    using intMemberPointerTraits = cero::variable_pointer_traits<&MyClass::intMember>;
    static_assert(std::is_same_v<intMemberPointerTraits::type, int>, "Error: variable_pointer_traits<&MyClass::intMember>::type should be int");
    static_assert(intMemberPointerTraits::is_member, "Error: variable_pointer_traits<&MyClass::intMember>::is_member should be true");
    static_assert(std::is_same_v<intMemberPointerTraits::clazz, MyClass>, "Error: variable_pointer_traits<&MyClass::intMember>::clazz should be MyClass");

    std::cout << "All static assertions passed." << std::endl;
    return 0;
}
