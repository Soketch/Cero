#include "sqlpool/Dbconn.h"
#include "sqlpool/ConnectionPool.h"
#include <jsoncpp/json/json.h>
#include <thread>
/*
1.单线程：使用/不使用连接池
    不使用：非连接池，单线程，用时：42693912629纳秒，42693毫秒
    使用：连接池，单线程，用时：9150030844纳秒，9150毫秒
2.多线程：使用/不使用连接池
    不使用：非连接池，多线程，用时：60792322338纳秒，60792毫秒
    使用：连接池，多线程，用时：3522916191纳秒，3522毫秒
*/
void op1(int begin, int end)
{
    for (int i = begin; i < end; i++)
    {
        cero::Dbconn conn;
        conn.CreateConnection("root", "Ydx2019!", "test", "127.0.0.1");
        std::string sql = "insert into person (first_name,last_name,age,email) values ('test','t',66,'test.tmp@hotfix.xom')";
        conn.updateDB(sql);
    }
}

void op2(cero::Connectionpool *pool, int begin, int end)
{
    for (int i = begin; i < end; i++)
    {
        std::shared_ptr<cero::Dbconn> conn = pool->getConnection();
        std::string sql = "insert into person (first_name,last_name,age,email) values ('test','t',66,'test.tmp@hotfix.xom')";
        conn->updateDB(sql);
    }
}

void test1()
{
#if 0
    // 非连接池，单线程，用时：37389973159纳秒，37389毫秒
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    op1(0, 5000);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    std::cout << "非连接池，单线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << std::endl;
#else
    // 连接池，单线程，用时：12089946216纳秒，12089毫秒
    auto pool = cero::Singleton<cero::Connectionpool>::GetInstance();
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    op2(pool, 0, 5000);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    auto length = end - begin;
    std::cout << "连接池，单线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << std::endl;
#endif
}

void test2()
{
#if 0
    // 非连接池，多线程，用时：8516567706纳秒，8516毫秒
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::thread t1(op1, 0, 1000);
    std::thread t2(op1, 1000, 2000);
    std::thread t3(op1, 2000, 3000);
    std::thread t4(op1, 3000, 4000);
    std::thread t5(op1, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    std::cout << "非连接池，多线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << std::endl;
#else
    // 连接池，多线程，用时：4341067792纳秒，4341毫秒
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto pool = cero::Singleton<cero::Connectionpool>::GetInstance();
    std::thread t1(op2, pool, 0, 1000);
    std::thread t2(op2, pool, 1000, 2000);
    std::thread t3(op2, pool, 2000, 3000);
    std::thread t4(op2, pool, 3000, 4000);
    std::thread t5(op2, pool, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto length = end - begin;
    std::cout << "连接池，多线程，用时：" << length.count() << "纳秒，" << length.count() / 1000000 << "毫秒" << std::endl;
#endif
}

int main()
{
    test2();
    return 0;
}