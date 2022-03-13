cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(rOpenCL)
set(CMAKE_MACOSX_RPATH 1)
option(BUILD_SHARED_LIBS "ON=Build shared libs, OFF=static libs" ON)
include_directories("fix-include" "include")
find_library(OPENCL_LIBRARIES OpenCL)
set(CMAKE_REQUIRED_LIBRARIES "${OPENCL_LIBRARIES}")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -fPIC")
set(DEFAULT_KERNEL_C_FLAGS "-O3")
set(POCL_LIBRARY_NAME "rOpenCL")

add_subdirectory("include")
add_subdirectory("lib/CL")




