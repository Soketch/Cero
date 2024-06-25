/**
 * @file Dbconn.h
 * @author {soketch} ({skgfwb@gmail.com})
 * @brief mysql连接类
 * @version 0.1
 * @date 2024-06-26
 */

#ifndef __DBCONN_H_
#define __DBCONN_H_

#include <mysql/mysql.h>
#include <iostream>
#include <chrono>
#include <cstdint>

namespace cero
{
    class Dbconn
    {
    public:
        Dbconn();

        ~Dbconn();

        // 连接数据库
        bool CreateConnection(std::string user, std::string passwd, std::string dbName, std::string ip = "127.0.0.1", uint16_t port = 3306);

        // 更新数据库操作，insert、update、delete
        bool updateDB(std::string sql);

        // 查询数据库操作select
        bool queryDB(std::string sql);

        // 遍历查询到的结果集
        bool next();

        // 得到结果集的字段值
        std::string value(int index);

        // 事务操作
        bool transaction();

        // 提交事务
        bool commit();

        // 事务回滚
        bool rollback();

        // 刷新起始空间时间点
        void refreshAlive();

        // 计算连接存活的时间
        int64_t getAliveTime();

        void setFieldCount(int v) { m_fieldCount = v; }
        // 获取结果集表行的字段数
        int getFieldCount() const { return m_fieldCount; }

    private:
        void freeResult();
        MYSQL *m_conn = nullptr;
        MYSQL_RES *m_res = nullptr;
        MYSQL_ROW m_row = nullptr;

        std::chrono::steady_clock::time_point m_alivetime;
        // 表行字段数
        int m_fieldCount = 0;
    };
}

#endif