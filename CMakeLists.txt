cmake_minimum_required(VERSION 3.24)
project(renderer)

set(CMAKE_CXX_STANDARD 23)
set(OpenCV_DIR "C:\\src\\opencv\\mingw-build\\install")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(renderer main.cpp src/renderer.cpp src/renderer.h
        src/test.h)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

target_link_libraries(renderer ${OpenCV_LIBS} OpenMP::OpenMP_CXX)
