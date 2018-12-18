cmake_minimum_required(VERSION 3.11)
project(nalp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

# Local path to luajit
set(ENV{PATH} ${PROJECT_SOURCE_DIR}/libs/luajit)

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
load("uws/src")
load("uws/uSockets/src")

include(FindLuaJIT)
include(FindOpenSSL)
include(FindUSockets)

include_directories(${LUAJIT_INCLUDE_DIR})

# Build the nalp test executable
add_executable(tests tests/index.cpp)

# Link luajit library
target_link_libraries(tests ${LUAJIT_LIBRARIES})

# Build nalp executable
add_executable(nalpexe src/app.cpp)

# Link luajit library
target_link_libraries(nalpexe ${LUAJIT_LIBRARIES})

# Build the UWS test executable
add_executable(helloWorld libs/uws/examples/HelloWorld.cpp ${USOCKETS_SOURCES})
target_link_libraries(helloWorld ${OPENSSL_LIBRARIES})


# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug)

