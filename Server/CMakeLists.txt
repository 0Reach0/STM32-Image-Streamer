cmake_minimum_required(VERSION 3.10)

project(STM32-Image-Streamer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug) 
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
include_directories(Inc)
find_package(PNG REQUIRED)

add_executable(Server Src/Logic.cpp  Src/General.cpp)
target_link_libraries(Server sfml-graphics sfml-window sfml-system PNG::PNG)
#target_link_libraries(Server sfml-graphics sfml-window sfml-system -lpng)
#target_link_libraries(Server sfml-graphics sfml-window sfml-system /usr/lib/x86_64-linux-gnu/libpng.so)
