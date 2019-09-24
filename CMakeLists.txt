cmake_minimum_required(VERSION 3.14)

project(hm)

find_package(SQLite3 REQUIRED)

include_directories(SYSTEM . ${SQLite3_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_executable(hm main.cpp)
target_link_libraries(hm ${SQLite3_LIBRARIES})

add_executable(read_db read_db.cpp)
target_link_libraries(read_db ${SQLite3_LIBRARIES})

install(TARGETS hm DESTINATION bin)


