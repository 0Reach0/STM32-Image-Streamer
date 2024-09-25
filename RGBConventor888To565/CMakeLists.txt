cmake_minimum_required(VERSION 3.10)

project(STM32-Image-Streamer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)  

add_executable(RGBConventor RGBConventor888To565.c)
target_link_libraries(RGBConventor PRIVATE PNG::PNG JPEG::JPEG) 