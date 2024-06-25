#include "Dbconn.h"

namespace cero
{

    cero::Dbconn::Dbconn()
    {
        m_conn = mysql_init(nullptr);
        if (m_conn == nullptr)
        {
            std::cerr << "mysql conn fail, mysql_init error." << std::endl;
        }
        else
        {
            mysql_set_character_set(m_conn, "utf8");
        }
    }

    cero::Dbconn::~Dbconn()
    {
        if (m_conn)
        {
            mysql_close(m_conn);
        }
        freeResult();
    }

    bool cero::Dbconn::CreateConnection(std::string user, std::string passwd, std::string dbName, std::string ip, uint16_t port)
    {
        MYSQL *ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0);
        if (ptr == nullptr)
        {
            std::cerr << "connect " << dbName << " database fail, MySQL error: " << mysql_error(m_conn) << std::endl;
            return false;
        }

        // std::cout << dbName << " datebase connect success" << std::endl;
        return true;
    }

    bool cero::Dbconn::updateDB(std::string sql)
    {
        if (mysql_query(m_conn, sql.c_str()))
        { // 返回0 成功  返回非0失败
            std::cerr << "update query sql fail, MySQL error: " << mysql_error(m_conn) << std::endl;
            return false;
        }
        return true;
    }

    bool cero::Dbconn::queryDB(std::string sql)
    {
        freeResult();
        if (mysql_query(m_conn, sql.c_str()))
        { // 返回 0 说明成功    非0说明失败
            return false;
        }
        m_res = mysql_store_result(m_conn);
        return true;
    }

    bool cero::Dbconn::next()
    {
        if (m_res == nullptr)
        {
            std::cerr << "next() m_res == nullptr,  errno" << mysql_errno(m_conn) << std::endl;
            return false;
        }

        m_row = mysql_fetch_row(m_res);
        // 设置字段数
        setFieldCount(mysql_num_fields(m_res));

        if (m_row == nullptr)
        {
            return false;
        }
        return true;
    }

    std::string cero::Dbconn::value(int index)
    {
        int fieldCount = mysql_num_fields(m_res);
        if (index >= fieldCount || index < 0)
        {
            return std::string(); // 返回空字符串
        }

        char *val = m_row[index];
        uint32_t length = mysql_fetch_lengths(m_res)[index];

        return std::string(val, length);
    }

    bool Dbconn::transaction()
    {
        return mysql_autocommit(m_conn, false);
    }
    bool Dbconn::commit()
    {
        return mysql_commit(m_conn);
    }

    bool Dbconn::rollback()
    {
        return mysql_rollback(m_conn);
    }

    void cero::Dbconn::refreshAlive()
    {
        // 将当前时间点 设置为起始时间点
        m_alivetime = std::chrono::steady_clock::now();
    }

    int64_t cero::Dbconn::getAliveTime()
    {
        // 将当前的时间点 减去起始时间点
        std::chrono::nanoseconds res = std::chrono::steady_clock::now() - m_alivetime;

        // 得到的结果（差值）时间点 转换为 毫秒
        std::chrono::milliseconds millisec = std::chrono::duration_cast<std::chrono::milliseconds>(res);

        // 返回毫秒数
        return millisec.count();
    }

    void cero::Dbconn::freeResult()
    {
        if (m_res)
        {
            mysql_free_result(m_res);
        }
    }

}