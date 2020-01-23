cmake_minimum_required(VERSION 3.2)
project(hm VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fpic -Wall -Wextra -pedantic -O3")

add_subdirectory(thirdparty/sqlitecpp)
add_subdirectory(thirdparty/apathy)
add_subdirectory(thirdparty/clipp)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/thirdparty/sqlitecpp/include
)

add_subdirectory(src)
add_subdirectory(misc)

# Prepare nix expression
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pkgs/nix/hm.nix.in ${CMAKE_CURRENT_BINARY_DIR}/hm-${hm_VERSION}.nix)            

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dmitry Kolmakov <ds.kolmakov@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "hm - command line history manager for Bash.")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_SYSTEM_NAME "Linux-x86_64")
set(CPACK_PACKAGE_VERSION ${hm_VERSION})

INCLUDE(CPack)

