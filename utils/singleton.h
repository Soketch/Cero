/**
 * @file singleton.h
 * @author {soketch} ({skgfwb@gmail.com})
 * @brief 单例模式类封装
 * @version 0.1
 * @date 2024-06-26
 */

#ifndef __SINGLETON_H_
#define __SINGLETON_H_

#include <memory>
#include <iostream>
namespace cero
{
    template <class T, class X = void, int N = 0>
    class Singleton
    {
    public:
        static T *GetInstance()
        {
            static T v;
            return &v;
        }
    };

    template <class T, class X = void, int N = 0>
    class SingletonPtr
    {
    public:
        static std::shared_ptr<T> GetInstance()
        {
            static std::shared_ptr<T> v(new T);
            return v;
        }
    };
}

#endif