cmake_minimum_required(VERSION 3.11)
project(nalp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

# Loading all header-only deps
function(load path)
    include_directories(${PROJECT_SOURCE_DIR}/libs/${path})
endfunction()
load("catch2/single_include/catch2")
load("rang/include")
load("cpp-dotenv")
load("json")
load("serializer")
load("yaml-cpp/include/yaml-cpp/src")
load("yaml-cpp/src")

# Link lua jit sources
set(ENV{PATH} ${PROJECT_SOURCE_DIR}/libs/luajit)
include(FindLuaJIT)
include_directories(${LUAJIT_INCLUDE_DIR})

# The nalp executable
add_executable(nalp src/app.cpp)

# Link luajit library
target_link_libraries(nalp ${LUAJIT_LIBRARIES})

# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug)

