cmake_minimum_required(VERSION 3.15)

include("cmake/HunterGate.cmake")
include("cmake/DownloadProject.cmake")
include(FetchContent)
include(ExternalProject)
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.244.tar.gz"
        SHA1 "2c0f491fd0b80f7b09e3d21adb97237161ef9835"
)

project(RayMarching)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror=return-type -fconcepts -Wno-unknown-pragmas")

hunter_add_package(fmt)
find_package(fmt CONFIG REQUIRED)
hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)
hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)
hunter_add_package(tinyxml2)
find_package(tinyxml2 CONFIG REQUIRED)
hunter_add_package(imgui)
find_package(imgui CONFIG REQUIRED)

download_project(PROJ           observable
        GIT_REPOSITORY https://github.com/ddinu/observable.git
        GIT_TAG        master
        )
include_directories(${observable_SOURCE_DIR}/observable/include)

download_project(PROJ           utilities
        GIT_REPOSITORY https://github.com/PetrFlajsingr/Utilities.git
        GIT_TAG        master
        )
include_directories(${utilities_SOURCE_DIR})
file(GLOB_RECURSE utilities_SRC_LIST "${utilities_SOURCE_DIR}/*.cpp")


set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/cmake/SDL2)
find_package(SDL2 CONFIG REQUIRED)
FetchContent_Declare(SDL2CPP GIT_REPOSITORY https://github.com/dormon/SDL2CPP.git)
FetchContent_Declare(geGL GIT_REPOSITORY https://github.com/dormon/geGL.git)
FetchContent_GetProperties(SDL2CPP)

if (NOT SDL2CPP_POPULATED)
    FetchContent_Populate(SDL2CPP)
    add_subdirectory(${sdl2cpp_SOURCE_DIR} ${sdl2cpp_BINARY_DIR})
endif ()
FetchContent_GetProperties(geGL)
if (NOT geGL_POPULATED)
    FetchContent_Populate(geGL)
    add_subdirectory(${gegl_SOURCE_DIR} ${gegl_BINARY_DIR})
endif ()

add_executable(RayMarching main.cpp ${utilities_SRC_LIST})
target_link_libraries(RayMarching glm geGL::geGL SDL2CPP::SDL2CPP imgui::imgui tinyxml2)