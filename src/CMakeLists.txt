set(BINARY ${CMAKE_PROJECT_NAME})


file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(BIN_DEST_FOLDER "/mnt/d/Projects/rpi_cross_comp")
set(SOURCES ${SOURCES})
set(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/fronius.h)

if(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/arm-linux-gnueabi-g++")
	#-lpthread MUST be at the end of the list otherwise linking problems will occur
	set(LIB_TO_INCLUDE PocoJSON.a PocoNet.a PocoFoundation.a PocoData.a PocoDataSQLite.a yaml-cpp.a PocoUtil.a -lpthread -ldl)
else()
	set(LIB_TO_INCLUDE -lpthread -lPocoJSON -lPocoNet -lPocoFoundation  -lyaml-cpp -lPocoUtil)
endif()

SET(OBJS
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/sqlite3.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/Binder.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/Connector.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/Extractor.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/Notifier.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/SessionImpl.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/SQLiteException.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/SQLiteStatementImpl.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/Utility.o
  ${CMAKE_CURRENT_SOURCE_DIR}/../obj/Statement.o
)


add_executable(${BINARY} ${SOURCES} ${HEADER_FILES} ${OBJS})
SET_SOURCE_FILES_PROPERTIES(
  ${OBJS}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)
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
	#add_custom_command(TARGET ${BINARY}
  #                 POST_BUILD
  #                 COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BINARY}> ${BIN_DEST_FOLDER}
	#	   COMMENT "copying the output binary to destination folder: ${BIN_DEST_FOLDER}"
	#)
endif()

