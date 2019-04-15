cmake_minimum_required(VERSION 3.10)
project(rogue_likely)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(rogue_likely_lib STATIC
    lib/rogue_likely.cpp
)

enable_testing()
add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest-release-1.8.1")
add_executable(rogue_likely_test
    test/sample_test.cpp
)
target_link_libraries(rogue_likely_test LINK_PUBLIC rogue_likely_lib gtest gtest_main)

add_executable(rogue_likely main.cpp)
target_link_libraries(rogue_likely LINK_PUBLIC rogue_likely_lib)