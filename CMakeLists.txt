cmake_minimum_required(VERSION 3.20)

set(CUDA 1)

if (CUDA)
    add_compile_definitions(CUDA)
    project(smoke LANGUAGES CXX CUDA)
else ()
    project(smoke)
endif ()

add_subdirectory(fluid)

find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-O3  -Wall -Wextra -pthread")
set(CMAKE_CXX_STANDARD 17)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS window graphics)

link_libraries(GLEW OpenGL::GL sfml-window sfml-graphics Qt5::Core Qt5::Widgets)

add_executable(smoke main.cpp app/tools/Math.cpp app/tools/Math.h app/disp/SmokeWidget.cpp app/disp/SmokeWidget.h app/disp/SmokeShader.cpp app/disp/SmokeShader.h app/disp/SmokeRenderer.cpp app/disp/SmokeRenderer.h app/disp/MouseState.cpp app/disp/MouseState.h app/disp/MainWindow.cpp app/disp/MainWindow.h app/disp/SettingsWidget.cpp app/disp/SettingsWidget.h app/tools/ScopedTimer.cpp app/tools/ScopedTimer.h app/disp/DispSettings.h app/disp/Shaders/SmokeVertexShader.h app/disp/Shaders/SmokeFragmentShader.h app/tools/Profile.h app/tools/ThreadPool.cpp app/tools/ThreadPool.h app/tools/ThreadSettings.h app/tools/ThreadSettings.cpp app/disp/LineShader.cpp app/disp/LineShader.h app/disp/Shaders/PlainFragmentShader.h app/disp/Shaders/PlainVertexShader.h app/disp/LineRenderer.cpp app/disp/LineRenderer.h app/tools/Ellipse.cpp app/tools/Ellipse.h app/tools/Point2F.cpp app/tools/Point2F.h app/disp/DispSettings.cpp app/tools/Ellipse.cpp app/tools/Ellipse.h app/tools/Point2F.cpp app/tools/Point2F.h app/disp/RendererBase.cpp app/disp/RendererBase.h app/disp/ShaderBase.cpp app/disp/ShaderBase.h app/State.h app/State.cpp)

target_link_libraries(smoke FluidLib)
