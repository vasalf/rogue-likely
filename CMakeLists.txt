cmake_minimum_required(VERSION 3.10)
project(rogue_likely)

set(CMAKE_CXX_STANDARD 17)

include(FindCurses)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/third_party/CLI11-release-1.7.1)

add_library(rogue_likely_lib STATIC
    lib/controller/controller.cpp
    lib/object/alive_object.cpp
    lib/object/object.cpp
    lib/object/object_fwd.cpp
    lib/ui/abstract_ui.cpp
    lib/ui/ncurses/ncurses_ui.cpp
    lib/ui/user_action.cpp
    lib/world/cell.cpp
    lib/world/world.cpp
    lib/world/world_generator.cpp
    lib/world/world_view.cpp
    lib/world/position.cpp
    lib/rogue_likely.cpp
)
target_link_libraries(rogue_likely_lib LINK_PUBLIC ${CURSES_LIBRARIES})

enable_testing()
add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest-release-1.8.1")
add_executable(rogue_likely_test
    test/sample_test.cpp
    test/cell_test.cpp
)
target_link_libraries(rogue_likely_test LINK_PUBLIC rogue_likely_lib gtest gtest_main)

add_executable(rogue_likely main.cpp)
target_link_libraries(rogue_likely LINK_PUBLIC rogue_likely_lib)