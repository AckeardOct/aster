cmake_minimum_required(VERSION 3.5)

project(aster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CONAN_DIR ${CMAKE_SOURCE_DIR}/build/conan)
include(${CONAN_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


set(GTEST_DIR ${CMAKE_SOURCE_DIR}/build/gtest)
link_directories(${GTEST_DIR}/build/lib)
include_directories(${GTEST_DIR}/googletest/include)


set(LUA_DIR ${CMAKE_SOURCE_DIR}/build/lua-5.3.5)
link_directories(${LUA_DIR}/install/lib)
include_directories(${LUA_DIR}/install/include)


set(PROJECT_DEPS
    ${CONAN_LIBS}
    gtest
    lua)

set(common-src
    src/common/game_config.cpp
    src/common/game_window.cpp
    src/common/math_utils.cpp)

set(components-src
    src/components/cmp_basic.cpp
    src/components/cmp_render.cpp)

set(scenes-src
    src/scenes/scn_asteroids.cpp)

set(systems-src
    src/systems/sys_basic.cpp
    src/systems/sys_render.cpp)

set(all-src
    src/main.cpp
    ${common-src}
    ${components-src}
    ${scenes-src}
    ${systems-src})

add_executable(${PROJECT_NAME} ${all-src})
target_link_libraries(${PROJECT_NAME} ${PROJECT_DEPS})
