/**
 * @file noncopyable.h
 * @author {soketch} ({skgfwb@gmail.com})
 * @brief 不可拷贝对象类封装
 * @date 2024-06-24
 */

// #pragma once
#ifndef __CERO_NONCOPYABLE_
#define __CERO_NONCOPYABLE_

namespace cero
{
    /**
     * @brief 不可拷贝、赋值
     */
    class Noncopyable
    {
    public:
        /**
         * @brief 默认构造函数
         */
        Noncopyable() = default;

        /**
         * @brief 默认析构函数
         */
        ~Noncopyable() = default;

        /**
         * @brief 禁用拷贝构造函数
         */
        Noncopyable(const Noncopyable &) = delete;

        /**
         * @brief 禁用赋值运算符
         */
        Noncopyable operator=(const Noncopyable &) = delete;

    private:
    };

}
#endif