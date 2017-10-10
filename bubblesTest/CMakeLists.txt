cmake_minimum_required (VERSION 2.8.11)
project (bubbles-test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(${PROJECT_NAME} coordscalculator_test.cpp)

add_subdirectory (googletest)
include_directories(${gtest_SOURCE_DIR}/include
                    ${gtest_SOURCE_DIR}
                    ../calculator
                    ..)
target_link_libraries (${PROJECT_NAME} gtest gmock)


