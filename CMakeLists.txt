cmake_minimum_required(VERSION 3.2)

project(hm VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_subdirectory(thirdparty/sqlite3)
add_subdirectory(thirdparty/apathy)
add_subdirectory(src)
add_subdirectory(misc)
