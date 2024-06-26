#ifndef __CONNECTIONPOOL_H_
#define __CONNECTIONPOOL_H_
#include "Dbconn.h"
#include "utils/noncopyable.h"
#include "utils/singleton.h"
#include <cstdint>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <jsoncpp/json/json.h>

namespace cero
{
    class Connectionpool
    {

    public:
        using ptr = std::shared_ptr<Connectionpool>;

        // 获取连接池中的连接
        std::shared_ptr<Dbconn> getConnection();

        ~Connectionpool();

    private:
        // 解析json文件
        bool parserJsonFile();

        // 任务函数 - 生产数据库连接
        void produceConnection();
        // 任务函数 - 销毁数据库连接
        void recycleConnection();

        // 增加一个连接
        void addConnection();

    private:
        // 构造函数私有-防止外部创建实例， 设置友元智能通过singleton创建
        // 最好的方法 - 是通过 manager  然后singleton
        Connectionpool();
        friend class SingletonPtr<Connectionpool>;
        friend class Singleton<Connectionpool>;

    private:
        std::string m_ip;
        std::string m_username;
        std::string m_password;
        std::string m_dbName;
        uint16_t m_port;
        int m_maxSize; // 最大连接数
        int m_minSize; // 最小连接数

        int m_timeout;     // 超时时长
        int m_maxIdletime; // 最长空闲时间

        std::mutex m_mutex;             // 互斥量
        std::condition_variable m_cond; // 条件变量

        std::queue<Dbconn *> m_connQueue; // 连接池容器
    };

    using SqlPoolPtr = SingletonPtr<Connectionpool>;
    using SqlPool = Singleton<Connectionpool>;
}
#endif