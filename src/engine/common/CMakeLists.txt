cmake_minimum_required(VERSION 3.5)

project(common LANGUAGES CXX)


GET_PROPERTY(GL_ENGINE_3RD GLOBAL PROPERTY GL_ENGINE_3RD)
GET_PROPERTY(GL_SRC_DIR GLOBAL PROPERTY GL_SRC_DIR)


set(src 
    game_config.cpp
    game_window.cpp
    math_utils.cpp
    file_system.cpp

    gen/gen_game_config.cpp
)


add_library(${PROJECT_NAME} ${src})
add_library(engine::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${GL_ENGINE_3RD} engine::scenes)
