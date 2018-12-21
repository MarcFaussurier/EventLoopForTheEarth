cmake_minimum_required(VERSION 3.11)
project(nalp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

# Local path to luajit
set(ENV{PATH} ${PROJECT_SOURCE_DIR}/libs/luajit)

include(FindLuaJIT)
include(FindOpenSSL)
# include(FindUSockets)

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
include_directories(${LUAJIT_INCLUDE_DIR})

find_package(ZLIB)

# Build the UWS test executable
add_executable(tests tests/index.cpp)
target_link_libraries(tests ${LUAJIT_LIBRARIES})
target_link_libraries(tests ${OPENSSL_LIBRARIES})
target_link_libraries(tests ${ZLIB_LIBRARIES})
target_link_libraries(tests pthread)

# Build the UWS main executable
add_executable(nalpexe src/app.cpp)# ${USOCKETS_SOURCES} src/Components/WSMessageToData.cpp src/Components/WSMessageToData.cpp src/Components/WSMessageToData.h src/Workers/Websocket.cpp src/Workers/Websocket.h src/Workers/Http.cpp src/Workers/Http.h src/Workers/TCP.cpp src/Workers/TCP.h src/Components/Worker.cpp src/Components/Worker.h src/Components/Kernel.cpp src/Components/Kernel.h)
target_link_libraries(nalpexe ${LUAJIT_LIBRARIES})
target_link_libraries(nalpexe ${OPENSSL_LIBRARIES})
target_link_libraries(nalpexe ${ZLIB_LIBRARIES})
target_link_libraries(nalpexe pthread)

# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /cmake-build-debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug)

