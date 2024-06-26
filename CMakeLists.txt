## 最小cmake版本
cmake_minimum_required(VERSION 3.22.1)

## 项目名
project(Cero)


## 设置 c++版本
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
## 项目文件夹
include_directories(${PROJECT_SOURCE_DIR})

## 设置构建项目编译条件
option(BUILD_CEROS off)

##  用到的库
include_directories(/usr/include/mysql)
include_directories(/usr/include/jsoncpp/json)
set( LIBS
    mysqlclient
    jsoncpp
    stdc++fs  # 添加此行以链接 std::experimental::filesystem
)

## 编译源文件
set(CERO_SOURCES
    sqlpool/Dbconn.cpp
    sqlpool/ConnectionPool.cpp
    utils/util.cpp
)


#调试
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

## 输出路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

## 编译 - 链接
if(${BUILD_CEROS})
add_executable(test_conn tests/test_conn.cpp ${CERO_SOURCES})
target_link_libraries(test_conn ${LIBS})

add_executable(test_pool tests/test_pool.cpp ${CERO_SOURCES})
target_link_libraries(test_pool ${LIBS})

add_executable(test_json tests/test_json.cpp ${CERO_SOURCES})
target_link_libraries(test_json ${LIBS})

else()


endif()

