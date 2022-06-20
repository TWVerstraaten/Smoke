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
find_package(SFML COMPONENTS window graphics)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-O3  -Wall -Wextra -pthread")
set(CMAKE_CXX_STANDARD 17)

link_libraries(OpenGL::GL sfml-window sfml-graphics Qt5::Core Qt5::Widgets)

add_executable(smoke main.cpp
        app/math/Math.cpp app/math/Math.h app/disp/SmokeWidget.cpp app/disp/SmokeWidget.h app/disp/SmokeShader.cpp app/disp/SmokeShader.h app/disp/SmokeRenderer.cpp app/disp/SmokeRenderer.h app/disp/MouseState.cpp app/disp/MouseState.h app/disp/MainWindow.cpp app/disp/MainWindow.h app/disp/SettingsWidget.cpp app/disp/SettingsWidget.h app/prf/ScopedTimer.cpp app/prf/ScopedTimer.h app/disp/DispSettings.h app/disp/Shaders/SmokeVertexShader.h app/disp/Shaders/SmokeFragmentShader.h app/prf/Profile.h app/thr/ThreadPool.cpp app/thr/ThreadPool.h app/thr/ThreadSettings.h app/thr/ThreadSettings.cpp app/disp/LineShader.cpp app/disp/LineShader.h app/disp/Shaders/PlainFragmentShader.h app/disp/Shaders/PlainVertexShader.h app/disp/LineRenderer.cpp app/disp/LineRenderer.h app/math/Ellipse.cpp app/math/Ellipse.h app/disp/DispSettings.cpp app/math/Ellipse.cpp app/math/Ellipse.h app/disp/RendererBase.cpp app/disp/RendererBase.h app/disp/ShaderBase.cpp app/disp/ShaderBase.h app/State.h app/State.cpp app/audio/AudioPlayer.cpp app/audio/AudioPlayer.h app/audio/LinearBuffer.h app/audio/CircularBuffer.h app/audio/SoundWindow.cpp app/audio/SoundWindow.h app/audio/Functions.h app/audio/Complex.h app/audio/BufferBase.h app/audio/Functions.cpp app/audio/Types.h app/disp/AudioWidget.cpp app/disp/AudioWidget.h app/disp/SpectrumWidget.cpp app/disp/SpectrumWidget.h app/disp/HistoryWidget.cpp app/disp/HistoryWidget.h app/disp/AudioWidgetBase.cpp app/disp/AudioWidgetBase.h app/disp/BeatWidget.cpp app/disp/BeatWidget.h app/audio/BeatHandler.h app/audio/BeatHandler.cpp app/disp/AutoMover.h app/disp/AutoMover.cpp app/math/Vector2F.h app/math/Vector2F.cpp app/disp/ManipulatorBase.h app/misc/Overloaded.h)

target_link_libraries(smoke FluidLib fftw3 sfml-audio)
