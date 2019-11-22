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


set(RTTR_DIR ${CMAKE_SOURCE_DIR}/build/rttr)
link_directories(${RTTR_DIR}/build/lib)
include_directories(${RTTR_DIR}/build/include)


set(PROJECT_DEPS
    ${CONAN_LIBS}
    gtest
    lua
    rttr_core)

include_directories(src)
include(src/common/src.cmake)
include(src/components/src.cmake)
include(src/scenes/src.cmake)
include(src/systems/src.cmake)
include(src/tests/src.cmake)

set(all-src
    src/main.cpp
    ${test-src}
    ${common-src}
    ${components-src}
    ${scenes-src}
    ${systems-src})

add_executable(${PROJECT_NAME} ${all-src})
target_link_libraries(${PROJECT_NAME} ${PROJECT_DEPS})
