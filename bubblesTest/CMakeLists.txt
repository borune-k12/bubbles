cmake_minimum_required (VERSION 2.8.11)
project (bubbles-test)

set(CMAKE_AUTOMOC ON)

add_executable(${PROJECT_NAME} tst_bubblestesttest.cpp)

add_subdirectory (googletest)
add_directory(../bubbles)
target_link_libraries (${PROJECT_NAME} gtest gmock)


