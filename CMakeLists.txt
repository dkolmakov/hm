cmake_minimum_required(VERSION 2.8)

project(hm)

find_package(SQLite3 REQUIRED)

include_directories(SYSTEM . ${SQLite3_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_executable(hm main.cpp)
target_link_libraries(hm ${SQLite3_LIBRARIES})

add_executable(hm-insert insert.cpp)
target_link_libraries(hm-insert ${SQLite3_LIBRARIES})

add_executable(hm-select select.cpp)
target_link_libraries(hm-select ${SQLite3_LIBRARIES})

install(TARGETS hm-insert hm-select DESTINATION bin)


