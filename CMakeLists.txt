cmake_minimum_required(VERSION 3.20)
project(smoke)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "-O3 -Werror -Wall -Wextra")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS window graphics)

link_libraries(GLEW OpenGL::GL sfml-window sfml-graphics)

add_executable(smoke main.cpp app/Fluid.cpp app/Fluid.h app/Matrix.cpp app/Matrix.h app/Settings.h app/Shader.cpp app/Shader.h app/VertexArray.cpp app/VertexArray.h app/MouseState.cpp app/MouseState.h app/Math.cpp app/Math.h)

