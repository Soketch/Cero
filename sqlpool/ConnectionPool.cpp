#include "ConnectionPool.h"
#include <thread>
#include <fstream>

namespace cero
{
    Connectionpool::Connectionpool()
    {
        if (!parserJsonFile())
        {
            std::cout << "parser json (mysql config file) fail." << std::endl;
            return;
        }
        // 解析成功后， 创建m_minSize个连接
        for (int i = 0; i < m_minSize; ++i)
        {
            addConnection();
        }

        // 生产者线程
        std::thread producer(&Connectionpool::produceConnection, this);

        // 回收线程 - 销毁连接
        std::thread recycler(&Connectionpool::recycleConnection, this);

        // 不阻塞主线程 -- 分离线程函数
        producer.detach();
        recycler.detach();
    }

    Connectionpool::~Connectionpool()
    {
        while (!m_connQueue.empty())
        {
            Dbconn *conn = m_connQueue.front();
            m_connQueue.pop();
            delete conn;
        }
    }

    std::shared_ptr<Dbconn> cero::Connectionpool::getConnection()
    {
        //// 当连接池中没有可用（空闲）的连接 - 需要阻塞一会儿 消费者线程

        std::unique_lock<std::mutex> locker(m_mutex);

        // 当连接池为空
        while (m_connQueue.empty())
        {
            // 阻塞时长为 m_timeout的一半
            if (std::cv_status::timeout == m_cond.wait_for(locker, std::chrono::milliseconds(m_timeout / 2)))
            {
                if (m_connQueue.empty())
                {
                    // return nullptr;
                    continue;
                }
            }
        }

        // 有可用连接取出使用，并设置用完放回连接池
        std::shared_ptr<Dbconn> connptr(m_connQueue.front(),
                                        [this](Dbconn *conn)
                                        {
                                            m_mutex.lock();
                                            conn->refreshAlive();   // 刷新起始空间时间点
                                            m_connQueue.push(conn); // 放入连接池
                                            m_mutex.unlock();
                                        });
        m_connQueue.pop();
        m_cond.notify_all();
        return connptr;
    }

    bool Connectionpool::parserJsonFile()
    {
        std::ifstream ifs("/home/coding/cpp/Cero/bin/conf/dbconfig.json");
        Json::Reader rd;
        Json::Value root;
        rd.parse(ifs, root);
        if (root.isObject())
        {
            m_ip = root["ip"].asString();
            m_port = root["port"].asInt();
            m_username = root["userName"].asString();
            m_password = root["password"].asString();
            m_dbName = root["dbName"].asString();
            m_minSize = root["minSize"].asInt();
            m_maxSize = root["maxSize"].asInt();
            m_maxIdletime = root["maxIdleTime"].asInt();
            m_timeout = root["timeout"].asInt();
            return true;
        }
        return false;
    }

    void Connectionpool::produceConnection()
    {
        while (true)
        {
            std::unique_lock<std::mutex> locker(m_mutex);
            while ((int)m_connQueue.size() < m_minSize)
            {
                addConnection();
                m_cond.notify_all(); // 唤醒消费者线程
            }
            m_cond.wait(locker);
        }
    }

    void Connectionpool::recycleConnection()
    {
        while (true)
        {

            // 每个0.5秒检测一次
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            std::lock_guard<std::mutex> locker(m_mutex);
            while ((int)m_connQueue.size() > m_maxSize)
            {
                // 取出对头， 对头最早创建，说明空闲时间长
                Dbconn *conn = m_connQueue.front();
                // 判断空闲时间是否超出
                if (conn->getAliveTime() >= m_maxIdletime)
                {
                    m_connQueue.pop();
                    delete conn;
                    std::cout << "清除一个m_connQueue中的连接" << std::endl;
                }
                else
                {
                    break;
                }
            }
        }
    }
    void Connectionpool::addConnection()
    {
        Dbconn *conn = new Dbconn;
        conn->CreateConnection(m_username, m_password, m_dbName, m_ip, m_port);
        conn->refreshAlive();
        m_connQueue.push(conn);
    }
}