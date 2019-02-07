cmake_minimum_required(VERSION 3.11)
project(nalp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)
# Local path to the local luajit repo
set(ENV{PATH} ${PROJECT_SOURCE_DIR}/libs/luajit)

set(PREFIX ${PROJECT_SOURCE_DIR}/libs/)

# Loading all dependencies
include(FindLuaJIT)
include(FindOpenSSL)
include_directories("${PREFIX}catch2/single_include/catch2")
include_directories("${PREFIX}rang/include")
include_directories("${PREFIX}cpp-dotenv")
include_directories("${PREFIX}json")
include_directories("${PREFIX}promise")
include_directories("${PREFIX}serializer")
include_directories(${LUAJIT_INCLUDE_DIR})
find_package(ZLIB)

# Build the tests executable
add_executable(tests
        src/event_engine/action.h       src/event_engine/bpromise.h
        src/event_engine/EventLoop.h    src/event_engine/EventLoop.cpp
        src/event_engine/Reactor.h      src/event_engine/Reactor.cpp
        src/event_engine/Profiler.cpp   src/event_engine/Profiler.h
        src/event_engine/window.cpp     src/event_engine/IDKParser.cpp
        src/event_engine/IDKParser.h    src/event_engine/ThreadGroup.cpp
        src/event_engine/ThreadGroup.h  src/event_engine/trim.h
        tests/index.cpp)

target_link_libraries(tests ${LUAJIT_LIBRARIES})
target_link_libraries(tests ${OPENSSL_LIBRARIES})
target_link_libraries(tests ${ZLIB_LIBRARIES})
target_link_libraries(tests pthread)

# Build the main executable
add_executable(nalpexe src/app.cpp
        src/util/AssociativeArray.h     src/util/AssociativeArray.hpp
        src/event_engine/action.h       src/event_engine/bpromise.h
        src/event_engine/EventLoop.h    src/event_engine/EventLoop.cpp
        src/event_engine/Reactor.h      src/event_engine/Reactor.cpp src/event_engine/Profiler.cpp src/event_engine/Profiler.h src/event_engine/window.cpp src/event_engine/IDKParser.cpp src/event_engine/IDKParser.h src/event_engine/ThreadGroup.cpp src/event_engine/ThreadGroup.h src/event_engine/trim.h)
target_link_libraries(nalpexe ${LUAJIT_LIBRARIES})
target_link_libraries(nalpexe ${OPENSSL_LIBRARIES})
target_link_libraries(nalpexe ${ZLIB_LIBRARIES})
target_link_libraries(nalpexe pthread)

# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug)
