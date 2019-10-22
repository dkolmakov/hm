cmake_minimum_required(VERSION 3.2)

project(hm VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

message("CMAKE_PROJECT_VERSION = ${PROJECT_VERSION}")

# Add a custom command that produces version.cpp, plus
# a dummy output that's not actually produced, in order
# to force version.cmake to always be re-run before the build
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.hpp
           ${CMAKE_CURRENT_BINARY_DIR}/_version.hpp
    COMMAND ${CMAKE_COMMAND} -D VERSION=${PROJECT_VERSION} -P
            ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)
            
add_custom_target(version_header ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/version.hpp)

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
