cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_FLAGS "-O3  -Wall -Wextra")

if (CUDA)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

    add_library(FluidLib Fluid.cuh Fluid.cu Matrix.cpp Matrix.h FluidSettings.h FluidSettings.cpp)
    set_property(TARGET FluidLib PROPERTY LINKER_LANGUAGE CUDA)
    set_target_properties(FluidLib PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
else ()
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
    set(CMAKE_CXX_STANDARD 17)

    link_libraries(Qt5::Core)

    add_library(FluidLib Fluid.hpp Fluid.cpp Matrix.cpp Matrix.h FluidSettings.h FluidSettings.cpp)
endif ()

