/**
 * @file Dbconn.h
 * @author {soketch} ({skgfwb@gmail.com})
 * @brief mysql连接类
 * @version 0.1
 * @date 2024-06-26
 */

#ifndef __DBCONN_H_
#define __DBCONN_H_

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <iostream>
#include <chrono>

namespace cero
{
    class Dbconn
    {
    public:
        Dbconn();

        ~Dbconn();

        // 连接数据库
        bool CreateConnection();

        // 更新数据库操作，insert、update、delete
        bool updateDB();

        // 查询数据库操作select
        bool queryDB();

        // 遍历查询到的结果集

        // 得到结果集的字段值

        // 事务操作

        // 提交事务

        // 事务回滚

        // 刷新起始空间时间点

        // 计算连接存活的时间

        // 获取结果集表行的字段数

    private:
        void freeResult();

        sql::mysql::MySQL_Driver *driver;
        sql::Connection *conn;
        sql::Statement *stmt;
        sql::ResultSet *res;
    };
}

#endif