cmake_minimum_required(VERSION 3.20)

if (CUDA)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set(CMAKE_CUDA_STANDARD 14)
    set(CMAKE_CUDA_STANDARD_REQUIRED TRUE)

    add_library(FluidLib Fluid.cuh Fluid.cu Matrix.cpp Matrix.h FluidSettings.h FluidSettings.cpp)
    set_property(TARGET FluidLib PROPERTY LINKER_LANGUAGE CUDA)
    #    set_target_properties(FluidLib PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
else ()
    set(CMAKE_CXX_STANDARD 17)

    add_library(FluidLib Fluid.hpp Fluid.cpp Matrix.cpp Matrix.h FluidSettings.h FluidSettings.cpp)
endif ()

