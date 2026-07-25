cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Convert Windows paths to CMake forward-slash paths so CMake can find GeodeConfig.cmake
if(DEFINED ENV{GEODE_SDK})
    file(TO_CMAKE_PATH "$ENV{GEODE_SDK}" GEODE_SDK_CLEAN)
    list(APPEND CMAKE_PREFIX_PATH "${GEODE_SDK_CLEAN}")
endif()

if(DEFINED ENV{CLI_PROFILE})
    file(TO_CMAKE_PATH "$ENV{CLI_PROFILE}" CLI_PROFILE_CLEAN)
    list(APPEND CMAKE_PREFIX_PATH "${CLI_PROFILE_CLEAN}/geode" "${CLI_PROFILE_CLEAN}/sdk")
endif()

find_package(Geode REQUIRED)

project(AILayoutGenerator VERSION 1.0.0 LANGUAGES CXX)

add_geode_mod(
    AILayoutGenerator
    SOURCES
        src/main.cpp
)
