cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)


GET_PROPERTY(GL_ENGINE_3RD GLOBAL PROPERTY GL_ENGINE_3RD)
GET_PROPERTY(GL_SRC_DIR GLOBAL PROPERTY GL_SRC_DIR)


set(GTEST_DIR ${CMAKE_SOURCE_DIR}/build/gtest)
link_directories(${GTEST_DIR}/build/lib)
include_directories(${GTEST_DIR}/googletest/include)


set(src 
    main.cpp
    test_lua.cpp
    test_sol2.cpp
    test_file_system.cpp
)


add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${GL_ENGINE_3RD} gtest engine::common)

























