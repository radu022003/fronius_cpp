cmake_minimum_required(VERSION 3.10)
#set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++)

project(FroniusClient)

set(CMAKE_CXX_STANDARD 14)
unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)
unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)

#set(CMAKE_CXX_TARGET_INCLUDE_PATH "/usr/local/include")

include_directories(src src/include include)
link_directories(lib/poco)
add_subdirectory(src)

if(NOT ${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/arm-linux-gnueabi-g++")
	ENABLE_TESTING()
	add_subdirectory(tst)
	add_subdirectory(lib/googletest)
endif()
