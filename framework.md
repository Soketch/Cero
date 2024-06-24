## Cero -- C++实现的Mysql的ORM库

Ubuntu下安装 mysql-server
MySql的connector/C++开发库 libmysqlcppconn-dev
>> apt install libmysqlcppconn-dev

🖍️🛠️技术：C/C++、CMake、Mysql.

🗂️目录架构：
```
Cero ---+
        |
        +--- cmake/   cmake函数
        |
        +--- models/  模型目录
        |
        +--- Sql/     数据库连接池和管理
        |
        +--- src/     源码目录
        |
        +--- tests/   测试目录
        |
        +--- build/   构建目录
        |
        ---- CMakeLists.txt
```