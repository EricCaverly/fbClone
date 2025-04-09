cmake_minimum_required(VERSION 3.10.0)
project(fb VERSION 0.1.0 LANGUAGES C CXX)

find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++20")
set (source_dir "${PROJECT_SOURCE_DIR}/src")

file (GLOB source_files "${source_dir}/*.cpp")

add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME} raylib)
