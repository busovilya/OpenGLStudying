project(OPENGL_EXPERIMENTS)

cmake_minimum_required(VERSION 3.16)

set(SOURCES src/shader.cpp src/shaderProgram.cpp src/texture.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_subdirectory(thirdparty/glm)

add_library(glad thirdparty/glad/glad.c)

include_directories(thirdparty/stb_image)
include_directories(thirdparty/glm)
include_directories(thirdparty/glad)
include_directories(thirdparty/)
include_directories(include/)

add_executable(main.out main.cpp ${SOURCES})
target_link_libraries(main.out glfw OpenGL::GL glad ${CMAKE_DL_LIBS})
set_target_properties(main.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)