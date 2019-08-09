cmake_minimum_required(VERSION 3.14)
project(lineFollowing)

set(CMAKE_CXX_STANDARD 14)

add_executable(lineFollowing main.cpp)


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


# linking
target_link_libraries(lineFollowing ${OpenCV_LIBS})
