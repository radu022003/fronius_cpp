set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})
target_compile_features(${BINARY} PUBLIC cxx_std_14)
add_compile_options(-std=c++14 -Wall -Wextra -g)
target_link_libraries(${BINARY} -lpthread -lPocoJSON -lPocoNet -lPocoFoundation -lyaml-cpp -lPocoUtil ${CMAKE_PROJECT_NAME}_lib gtest)
#target_include_directories(${BINARY} PUBLIC ${PROJECT_SOURCE_DIR}/src/include)


add_test(NAME ${BINARY} COMMAND ${BINARY})
#target_include_directories(${BINARY} PUBLIC ${PROJECT_SOURCE_DIR}/src/include)
target_link_libraries(${BINARY} -lpthread -lPocoJSON -lPocoNet -lPocoFoundation -lyaml-cpp -lPocoUtil ${CMAKE_PROJECT_NAME}_lib gtest)

