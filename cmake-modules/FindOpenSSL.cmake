find_package(PkgConfig REQUIRED)
pkg_search_module(# Loading all dependencies
        include(FindLuaJIT)
        include(FindOpenSSL)
        include_directories("${PREFIX}catch2/single_include/catch2")
        include_directories("${PREFIX}rang/include")
        include_directories("${PREFIX}cpp-dotenv")
        include_directories("${PREFIX}json")
        include_directories("${PREFIX}serializer")
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
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /cmake-build-debug) REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()