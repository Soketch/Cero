#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>

// 连接到数据库的函数
sql::Connection *connectToDatabase(const std::string &host, const std::string &user, const std::string &password, const std::string &schema)
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect(host, user, password);
    con->setSchema(schema);
    return con;
}

// 执行查询并打印结果的函数
void executeAndPrintQuery(sql::Connection *con, const std::string &query)
{
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery(query);

    sql::ResultSetMetaData *meta = res->getMetaData();
    int columnCount = meta->getColumnCount();

    // 打印列名
    for (int i = 1; i <= columnCount; ++i)
    {
        std::cout << meta->getColumnName(i) << "\t";
    }
    std::cout << std::endl;

    // 遍历结果集并打印每行的数据
    while (res->next())
    {
        for (int i = 1; i <= columnCount; ++i)
        {
            std::cout << res->getString(i) << "\t";
        }
        std::cout << std::endl;
    }

    // 清理
    delete res;
    delete stmt;
}

// 插入数据的函数
void insertData(sql::Connection *con, const std::string &insertQuery)
{
    sql::Statement *stmt = con->createStatement();
    stmt->execute(insertQuery);
    delete stmt;
}

int main()
{
    try
    {
        // 连接到数据库
        sql::Connection *con = connectToDatabase("tcp://127.0.0.1:3306", "root", "Ydx2019!", "cero");

        // 执行查询并打印结果
        executeAndPrintQuery(con, "SELECT * FROM test_person");

        // 插入数据示例
        insertData(con, "INSERT INTO test_person (name, age, birthdate, gender) VALUES ('John Doe', 30, '1990-01-01', 1)");

        // 清理
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
