cmake_minimum_required(VERSION 3.13)

project(AILayoutGenerator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Auto-locate GeodeConfig.cmake inside the Geode SDK directory
if(DEFINED ENV{GEODE_SDK})
    file(TO_CMAKE_PATH "$ENV{GEODE_SDK}" GEODE_SDK_PATH)
    
    find_path(GEODE_CONFIG_DIR GeodeConfig.cmake
        PATHS
            "${GEODE_SDK_PATH}"
            "${GEODE_SDK_PATH}/cmake"
            "${GEODE_SDK_PATH}/share/geode/cmake"
    )
    
    if(GEODE_CONFIG_DIR)
        set(Geode_DIR "${GEODE_CONFIG_DIR}" CACHE PATH "Geode SDK Directory" FORCE)
    endif()
endif()

find_package(Geode REQUIRED)

add_geode_mod(
    AILayoutGenerator
    SOURCES
        src/main.cpp
)
