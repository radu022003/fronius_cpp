set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})
target_compile_features(${BINARY} PUBLIC cxx_std_14)
add_compile_options(-std=c++14 -Wall -Wextra -g)
target_link_libraries(${BINARY} PUBLIC -lpthread -lPocoJSON -lPocoNet -lPocoFoundation -lPocoData -lPocoDataSQLite -lyaml-cpp -lPocoUtil ${CMAKE_PROJECT_NAME}_lib gtest gmock)
#target_include_directories(${BINARY} PUBLIC ${PROJECT_SOURCE_DIR}/src/include)
set_property(
    TARGET ${BINARY}
    PROPERTY INSTALL_RPATH
    "\$ORIGIN"
    "\$ORIGIN/../../lib/poco"
)


add_test(NAME ${BINARY} COMMAND ${BINARY})
#target_include_directories(${BINARY} PUBLIC ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(${BINARY} PUBLIC -lpthread -lPocoJSON -lPocoNet -lPocoFoundation -lPocoData -lPocoDataSQLite -lyaml-cpp -lPocoUtil ${CMAKE_PROJECT_NAME}_lib gtest)
set_property(
    TARGET ${BINARY}
    PROPERTY INSTALL_RPATH
    "\$ORIGIN"
    "\$ORIGIN/../../lib/poco"
    "\$ORIGIN/../../lib/yaml"
    "/usr/local/lib"
)
