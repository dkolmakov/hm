cmake_minimum_required(VERSION 2.8)

project(hm)

find_package(SQLite3 REQUIRED)
include_directories(SYSTEM ${SQLite3_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_executable(hm main.cpp)
target_link_libraries(hm ${SQLite3_LIBRARIES})

add_executable(create_table create_table.cpp)
target_link_libraries(create_table ${SQLite3_LIBRARIES})

