cmake_minimum_required(VERSION 3.5)

project(Aster-Solution)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set(CONAN_DIR ${CMAKE_SOURCE_DIR}/build/conan)
include(${CONAN_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


set(LUA_DIR ${CMAKE_SOURCE_DIR}/build/lua-5.3.5)
link_directories(${LUA_DIR}/install/lib)
include_directories(${LUA_DIR}/install/include)


set(SOL2_DIR ${CMAKE_SOURCE_DIR}/build/sol2)
include_directories(${SOL2_DIR}/single/single/include)


set(GL_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
SET_PROPERTY(GLOBAL PROPERTY GL_SRC_DIR ${GL_SRC_DIR})


set(GL_ENGINE_3RD
    ${CONAN_LIBS}
    lua
)
SET_PROPERTY(GLOBAL PROPERTY GL_ENGINE_3RD ${GL_ENGINE_3RD})


set(GL_ALL_ENGINE
    engine::common
    engine::components
    engine::scenes
    engine::systems
)
SET_PROPERTY(GLOBAL PROPERTY GL_ALL_ENGINE ${GL_ALL_ENGINE})


include_directories(src/engine)
add_subdirectory(src/engine)

add_subdirectory(src/aster)
add_subdirectory(src/tests)
add_subdirectory(src/playground/ogles)
