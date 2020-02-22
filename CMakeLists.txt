cmake_minimum_required(VERSION 3.15)

include("cmake/HunterGate.cmake")
include("cmake/DownloadProject.cmake")

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

add_executable(RayMarching main.cpp)
target_link_libraries(RayMarching glm)