#pragma once

#include <cassert>
#include <iostream>

#ifndef MIRROW_ASSERT
#define MIRROW_ASSERT(x, expr) assert(((void)(expr), (x)))
#endif

#ifndef MIRROW_LOG
#define MIRROW_LOG(...) std::cout << __VA_ARGS__ << std::endl;
#endif

/*

MIRROW_ASSERT(x, expr) : 这个宏用于替代标准库中的 assert 宏。它接受两个参数：
 x : 是一个断言的条件，通常是一个布尔表达式。
 expr : 是要执行的表达式。
  宏展开后会调用 assert 函数，确保在断言失败时打印消息并终止程序。使用(void)(expr) 可以确保 expr 在宏展开时被执行，避免了可能由于宏参数的副作用而导致的问题。

MIRROW_LOG(...) : 这个宏用于简化输出日志到标准输出的操作。它接受可变数量的参数，并使用 std::cout 将它们输出到控制台，每个参数之间用空格分隔，末尾附加换行符以便换行显示日志。
*/