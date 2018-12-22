cmake_minimum_required(VERSION 3.11)
project(nalp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

# Local path to the local luajit repo
set(ENV{PATH} ${PROJECT_SOURCE_DIR}/libs/luajit)

# Loading all dependencies
include(FindLuaJIT)
include(FindOpenSSL)
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
include_directories(${LUAJIT_INCLUDE_DIR})
find_package(ZLIB)

# Build the tests executable
add_executable(tests tests/index.cpp)
target_link_libraries(tests ${LUAJIT_LIBRARIES})
target_link_libraries(tests ${OPENSSL_LIBRARIES})
target_link_libraries(tests ${ZLIB_LIBRARIES})
target_link_libraries(tests pthread)

# Build the main executable
add_executable(nalpexe src/app.cpp)
target_link_libraries(nalpexe ${LUAJIT_LIBRARIES})
target_link_libraries(nalpexe ${OPENSSL_LIBRARIES})
target_link_libraries(nalpexe ${ZLIB_LIBRARIES})
target_link_libraries(nalpexe pthread)

# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug)

