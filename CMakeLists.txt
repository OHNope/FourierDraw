cmake_minimum_required(VERSION 3.19)
project(FourierDraw)

#find_library(OpenCV)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp main.hpp FFT.hpp Delaunay.hpp DouglasPeucker.hpp kruskal.hpp)

add_executable(FourierDraw ${SOURCE_FILES})
target_link_libraries( FourierDraw  ${OpenCV_LIBS} )