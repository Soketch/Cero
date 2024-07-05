#include <iostream>
#include "utils/function_traits.hpp" // 假设你的文件名为 function_traits.hpp

void test_function(int, double) {}

struct TestClass
{
    void member_function(int) {}
    void const_member_function(int) const {}
};

int main()
{
    // 测试普通函数类型
    using FuncType = cero::function_type_from_pointer_t<&test_function>;
    static_assert(std::is_same_v<FuncType, void(int, double)>, "Function type does not match");

    using FuncTraits = cero::function_pointer_traits<&test_function>;
    static_assert(std::is_same_v<FuncTraits::return_type, void>, "Return type does not match");
    static_assert(std::is_same_v<FuncTraits::args, cero::type_list<int, double>>, "Arguments type list does not match");

    // 测试类成员函数类型
    using MemberFuncType = cero::function_type_from_pointer_t<&TestClass::member_function>;
    static_assert(std::is_same_v<MemberFuncType, void (TestClass::*)(int)>, "Member function type does not match");

    using MemberFuncTraits = cero::function_pointer_traits<&TestClass::member_function>;
    static_assert(std::is_same_v<MemberFuncTraits::return_type, void>, "Return type does not match");
    static_assert(std::is_same_v<MemberFuncTraits::args, cero::type_list<int>>, "Arguments type list does not match");
    static_assert(std::is_same_v<MemberFuncTraits::args_with_class, cero::type_list<TestClass *, int>>, "Arguments with class type list does not match");
    static_assert(MemberFuncTraits::is_member, "Should be a member function");
    static_assert(!MemberFuncTraits::is_const, "Should not be const");

    // 测试类常成员函数类型
    using ConstMemberFuncType = cero::function_type_from_pointer_t<&TestClass::const_member_function>;
    static_assert(std::is_same_v<ConstMemberFuncType, void (TestClass::*)(int) const>, "Const member function type does not match");

    using ConstMemberFuncTraits = cero::function_pointer_traits<&TestClass::const_member_function>;
    static_assert(std::is_same_v<ConstMemberFuncTraits::return_type, void>, "Return type does not match");
    static_assert(std::is_same_v<ConstMemberFuncTraits::args, cero::type_list<int>>, "Arguments type list does not match");
    static_assert(std::is_same_v<ConstMemberFuncTraits::args_with_class, cero::type_list<TestClass *, int>>, "Arguments with class type list does not match");
    static_assert(ConstMemberFuncTraits::is_member, "Should be a member function");
    static_assert(ConstMemberFuncTraits::is_const, "Should be const");

    std::cout << "All static assertions passed." << std::endl;
    return 0;
}
