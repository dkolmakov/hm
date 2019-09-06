cmake_minimum_required(VERSION 2.8)

project(hm)

find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED
	COMPONENTS program_options
	)

include_directories(SYSTEM . ${SQLite3_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_executable(hm main.cpp)
target_link_libraries(hm ${SQLite3_LIBRARIES})

add_executable(hm-insert insert.cpp)
target_link_libraries(hm-insert ${Boost_LIBRARIES} ${SQLite3_LIBRARIES})

