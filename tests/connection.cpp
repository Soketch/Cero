#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

int main()
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;

    try
    {
        // 创建mysql连接
        driver = sql::mysql::get_driver_instance();
        conn = driver->connect("127.0.0.1:3306", "root", "Ydx2019!");

        // 选择数据库
        conn->setSchema("cero");

        // 执行简单查询
        stmt = conn->createStatement();
        res = stmt->executeQuery("SELECT * FROM test_person");

        // 处理结果集
        while (res->next())
        {
            std::cout << "ID: " << res->getInt("id")
                      << ", Name: " << res->getString("name")
                      << ", Age: " << res->getInt("age")
                      << ", Birthdate: " << res->getString("birthdate") // 使用 getString 以便输出日期
                      << ", Gender: " << res->getInt("gender")          // 没有 getTinyInt 方法，直接使用 getInt
                      << std::endl;
        }
        // 清理操作
        delete res;
        delete stmt;
        delete conn;
    }
    catch (const sql::SQLException &e)
    {

        std::cerr << "SQL Error:" << e.what() << '\n';
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // delete driver;
    return 0;
}