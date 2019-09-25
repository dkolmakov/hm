cmake_minimum_required(VERSION 2.8)

project(hm)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_executable(hm main.cpp)
add_executable(read_db read_db.cpp)

add_subdirectory(sqlite3)
target_include_directories(sqlite3 PUBLIC "${PROJECT_SOURCE_DIR}/sqlite3")
target_include_directories(hm PRIVATE "${PROJECT_SOURCE_DIR}/sqlite3")
target_include_directories(read_db PRIVATE "${PROJECT_SOURCE_DIR}/sqlite3")

target_link_libraries(read_db sqlite3)
target_link_libraries(hm sqlite3)

install(TARGETS hm DESTINATION bin)


