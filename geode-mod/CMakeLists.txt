cmake_minimum_required(VERSION 3.13)

project(AILayoutGenerator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

add_geode_mod(
    AILayoutGenerator
    SOURCES
        src/main.cpp
)
