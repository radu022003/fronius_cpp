set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})
#set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++)

add_executable(${BINARY} ${TEST_SOURCES})
target_compile_features(${BINARY} PUBLIC cxx_std_14)
add_compile_options(-std=c++14 -Wall -Wextra -g)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)
