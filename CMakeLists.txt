## 最小cmake版本
cmake_minimum_required(VERSION 3.22.1)

## 项目名
project(Cero)


## 设置 c++版本
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR})


##  用到的库
include_directories(/usr/include/mysql)
include_directories(/use/include/jsoncpp/json)
set( LIBS
    mysqlclient
    jsoncpp
)

## 编译源文件
set(CERO_SOURCES
    sqlpool/Dbconn.cpp
)


#调试
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


## 输出 - 链接

add_executable(test_conn tests/test_conn.cpp ${CERO_SOURCES})
target_link_libraries(test_conn ${LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)