cmake_minimum_required(VERSION 2.8)

project(hm)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_executable(hm-db main.cpp)
add_executable(read-db read_db.cpp)

add_subdirectory(sqlite3)
target_include_directories(sqlite3 PUBLIC "${PROJECT_SOURCE_DIR}/sqlite3")
target_include_directories(hm-db PRIVATE "${PROJECT_SOURCE_DIR}/sqlite3")
target_include_directories(read-db PRIVATE "${PROJECT_SOURCE_DIR}/sqlite3")

target_link_libraries(read-db sqlite3)
target_link_libraries(hm-db sqlite3)

install(TARGETS hm-db DESTINATION bin)


