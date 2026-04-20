
cmake_minimum_required(VERSION 3.21)
project(test_package LANGUAGES CXX)

find_package(tc REQUIRED CONFIG)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME} PRIVATE test_plugin.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE tc::common)
