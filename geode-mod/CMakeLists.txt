cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to find the Geode SDK from the environment variable
if(DEFINED ENV{GEODE_SDK})
    list(APPEND CMAKE_PREFIX_PATH $ENV{GEODE_SDK})
endif()

find_package(Geode REQUIRED)

project(AILayoutGenerator VERSION 1.0.0)

add_geode_mod(
    AILayoutGenerator
    SOURCES
        src/main.cpp
)
