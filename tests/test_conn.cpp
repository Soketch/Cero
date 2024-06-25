#include "sqlpool/Dbconn.h"

void test_conn()
{
    cero::Dbconn conn;
    conn.CreateConnection("root", "Ydx2019!", "test", "127.0.0.1", 3306);

    conn.queryDB("select * from user");

    while (conn.next())
    {
        for (int i = 0; i < conn.getFieldCount(); ++i)
        {
            std::string value = conn.value(i);
            std::cout << value << "  ";
        }
        std::cout << "\n";
    }
}

int main()
{
    test_conn();
    return 0;
}