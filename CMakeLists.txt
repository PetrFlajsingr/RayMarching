cmake_minimum_required(VERSION 3.15)
project(RayMarching)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror=return-type -fconcepts -Wno-unknown-pragmas")

include_directories(utils)
include_directories(utils/include)

add_executable(RayMarching main.cpp)