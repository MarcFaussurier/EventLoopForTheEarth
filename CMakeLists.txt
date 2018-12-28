cmake_minimum_required(VERSION 3.11)
project(nalp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

set(PREFIX ${PROJECT_SOURCE_DIR}/libs/)

include_directories("${PREFIX}catch2/single_include/catch2")
include_directories("${PREFIX}PREFIXrang/include")
include_directories("${PREFIX}PREFIXcpp-dotenv")
include_directories("${PREFIX}json")
include_directories("${PREFIX}serializer")
include_directories("${PREFIX}promise")

find_package(ZLIB)

# Build the tests executable
add_executable(tests tests/index.cpp)
target_link_libraries(tests ${ZLIB_LIBRARIES})
target_link_libraries(tests pthread)

# Build the main executable
add_executable(nalpexe src/app.cpp)
target_link_libraries(nalpexe ${ZLIB_LIBRARIES})
target_link_libraries(nalpexe pthread)

# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug)

