cmake_minimum_required(VERSION 3.13)
project(opencvTest)

set(CMAKE_CXX_STANDARD 11)


#include_directories(
#        /usr/local/include/opencv4
#)

link_directories(
        /usr/local/lib
        /home/php/opencv-4.1.1/platforms/linux/build_hardfp/lib
)
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(OpenCV_DIR /home/php/opencv-4.1.1/platforms/linux/build_hardfp)   #Arm版 添加CaffeConfig.cmake的搜索路径
endif ()

find_package(OpenCV REQUIRED)

add_executable(opencvTest main.cpp test.cpp)
target_link_libraries( opencvTest ${OpenCV_LIBS} )