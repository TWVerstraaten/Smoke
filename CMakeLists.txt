cmake_minimum_required(VERSION 3.20)
project(smoke)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "-O3 -Werror -Wall -Wextra")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(smoke main.cpp app/Fluid.cpp app/Fluid.h app/Matrix.cpp app/Matrix.h app/Settings.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} OpenGL::GL)
