cmake_minimum_required(VERSION 3.5)

project(scenes LANGUAGES CXX)


GET_PROPERTY(GL_ENGINE_3RD GLOBAL PROPERTY GL_ENGINE_3RD)
GET_PROPERTY(GL_SRC_DIR GLOBAL PROPERTY GL_SRC_DIR)


set(src 
    scn_asteroids.cpp
)


add_library(${PROJECT_NAME} OBJECT ${src})
add_library(engine::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${GL_ENGINE_3RD} engine::components)
