cmake_minimum_required(VERSION 3.5)

project(aster LANGUAGES CXX)


GET_PROPERTY(GL_ALL_ENGINE GLOBAL PROPERTY GL_ALL_ENGINE)
GET_PROPERTY(GL_SRC_DIR GLOBAL PROPERTY GL_SRC_DIR)


add_executable(${PROJECT_NAME} ${GL_SRC_DIR}/aster/main.cpp)
target_link_libraries(${PROJECT_NAME} ${GL_ALL_ENGINE} engine::common)
