## 最小cmake版本
cmake_minimum_required(VERSION 3.22.1)

## 项目名
project(Cero)

## 设置 c++版本
set(CMAKE_CXX_STANDARD 20)

link_directories(/usr/include)

# 手动设置MySQL Connector/C++的路径
set(MySQLConnectorCpp_INCLUDE_DIR /usr/include/cppconn)
set(MySQLConnectorCpp_LIB_DIR /usr/lib/x86_64-linux-gnu)

# 包含头文件目录
include_directories(${MySQLConnectorCpp_INCLUDE_DIR})

# 查找库文件
find_library(MySQLConnectorCpp_LIBRARY
  NAMES mysqlcppconn
  PATHS ${MySQLConnectorCpp_LIB_DIR}
)

# 如果找到了库文件，继续配置
if(MySQLConnectorCpp_LIBRARY)
  message(STATUS "Found MySQL Connector/C++ library: ${MySQLConnectorCpp_LIBRARY}")
else()
  message(FATAL_ERROR "Could not find MySQL Connector/C++ library")
endif()

set(SQL_LIBS
    sql/connection.cpp
)


# 添加源文件
add_executable(mysql_test sql/connection.cpp)

# 链接MySQL Connector/C++库
target_link_libraries(mysql_test ${MySQLConnectorCpp_LIBRARY} mysqlcppconn)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)