cmake_minimum_required(VERSION 3.10)
#set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++)

project(FroniusClient)

set(CMAKE_CXX_STANDARD 14)
unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)
unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)

#set(CMAKE_CXX_TARGET_INCLUDE_PATH "/usr/local/include")
set(CMAKE_VERBOSE_MAKEFILE ON)
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(src src/include include)
link_directories(lib/poco lib/yaml "\$ORIGIN")
add_subdirectory(src)

if(NOT ${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/arm-linux-gnueabi-g++")
	ENABLE_TESTING()
	add_subdirectory(tst)
	add_subdirectory(lib/googletest)
endif()
