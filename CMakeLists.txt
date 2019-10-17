cmake_minimum_required(VERSION 3.2)

project(hm VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_subdirectory(thirdparty/sqlite3)
add_subdirectory(thirdparty/apathy)
add_subdirectory(src)
add_subdirectory(misc)

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})

set (CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dmitry Kolmakov <ds.kolmakov@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "hm - command line history manager for Bash.")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

INCLUDE(CPack)
