cmake_minimum_required(VERSION 3.5)

project(ogles LANGUAGES CXX)


GET_PROPERTY(GL_ENGINE_3RD GLOBAL PROPERTY GL_ENGINE_3RD)
GET_PROPERTY(GL_SRC_DIR GLOBAL PROPERTY GL_SRC_DIR)


set(src
    main.cpp
)

add_subdirectory(stb-image)

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${GL_ENGINE_3RD} GL engine::common stb-image)
