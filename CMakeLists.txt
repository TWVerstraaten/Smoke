cmake_minimum_required(VERSION 3.20)
project(smoke)


find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-O3 -Werror -Wall -Wextra -pthread")
set(CMAKE_CXX_STANDARD 17)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS window graphics)

link_libraries(GLEW OpenGL::GL sfml-window sfml-graphics Qt5::Core Qt5::Widgets)

add_executable(smoke main.cpp app/fl/Fluid.cpp app/fl/Fluid.h app/fl/Matrix.cpp app/fl/Matrix.h app/fl/Settings.h app/Math.cpp app/Math.h app/disp/MainWidget.cpp app/disp/MainWidget.h app/disp/Shader.cpp app/disp/Shader.h app/disp/DrawingBuffers.cpp app/disp/DrawingBuffers.h app/disp/MouseState.cpp app/disp/MouseState.h app/disp/MainWindow.cpp app/disp/MainWindow.h)

