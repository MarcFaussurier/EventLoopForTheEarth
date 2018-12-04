cmake_minimum_required(VERSION 3.11)
project(cppapp)
set(CMAKE_CXX_STANDARD 17)

# Loading all deps
find_package (Threads)
function(load path)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/${path})
endfunction()
load("catch2/single_include/catch2")
load("rang/include")
load("cpp-dotenv")
load("json")
load("serializer")
load("yaml-cpp/include/yaml-cpp/src")
load("yaml-cpp/src")


# Project files
add_executable(cppapp src/app.cpp)
target_link_libraries(cppapp ${CMAKE_THREAD_LIBS_INIT})

# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug)

