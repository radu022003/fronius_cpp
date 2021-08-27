set(BINARY ${CMAKE_PROJECT_NAME})


file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(BIN_DEST_FOLDER "/mnt/d/Projects/rpi_cross_comp")
set(SOURCES ${SOURCES})
set(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/fronius.h)

if(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/arm-linux-gnueabi-g++")
	set(LIB_TO_INCLUDE  PocoJSON.a PocoNet.a PocoFoundation.a PocoData.a PocoDataSQLite.a yaml-cpp.a PocoUtil.a -lpthread)
else()
	set(LIB_TO_INCLUDE -lpthread -lPocoJSON -lPocoNet -lPocoFoundation -lPocoData -lPocoDataSQLite -lyaml-cpp -lPocoUtil)
endif()

add_executable(${BINARY} ${SOURCES} ${HEADER_FILES})
target_include_directories(${BINARY} PUBLIC ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(${BINARY} ${LIB_TO_INCLUDE})
target_compile_features(${BINARY} PUBLIC cxx_std_14)
add_compile_options(-std=c++14 -Wall -Wextra -g)

#display some infos
get_target_property(dirs ${BINARY} INCLUDE_DIRECTORIES)
message(STATUS "dirs: ${dirs}")
get_target_property(libs ${BINARY} LINK_LIBRARIES)
message(STATUS "libs: ${libs}")
message(STATUS "compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "linker: ${CMAKE_LINKER}")

#if not cross compile, than create the static library for gtest
#else strip the output binary and copy it to destination folder
if(NOT ${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/arm-linux-gnueabi-g++")
	add_library(${BINARY}_lib STATIC ${SOURCES})
	add_compile_options(-std=c++14 -Wall -Wextra -g)
	target_compile_features(${BINARY} PUBLIC cxx_std_14)
	target_link_libraries(${BINARY} ${LIB_TO_INCLUDE})
	target_include_directories(${BINARY} PUBLIC ${PROJECT_SOURCE_DIR}/src/include)
else()
	add_custom_command(TARGET ${BINARY}
                   POST_BUILD
                   COMMAND  /usr/bin/arm-linux-gnueabi-strip ${PROJECT_NAME}
		   COMMENT "stripping the binary"
		   VERBATIM
	)
	add_custom_command(TARGET ${BINARY}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BINARY}> ${BIN_DEST_FOLDER}
		   COMMENT "copying the output binary to destination folder: ${BIN_DEST_FOLDER}"
	)
endif()

