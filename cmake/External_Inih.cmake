include (ExternalProject)

ExternalProject_Add(Inih_ext
        GIT_REPOSITORY https://github.com/Enziferum/iniparser.git
        BINARY_DIR "${CMAKE_BINARY_DIR}/third-party/iniparser-build"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/third-party/iniparser-src"
        INSTALL_DIR "${CMAKE_BINARY_DIR}/third-party/iniparser-build"
        CMAKE_ARGS "${Inih_CMAKE_ARGS}"
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/third-party/iniparser-build
        -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
        INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
        UPDATE_COMMAND ""
        BUILD_COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . && ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE}
        )
set(Inih_INCLUDE_DIRS
        "${CMAKE_BINARY_DIR}/third-party/iniparser-build/include"
        CACHE PATH "Ini-parser include directory"
        )
# No link directories because header-only library
link_directories("${CMAKE_BINARY_DIR}/third-party/iniparser-build/lib")
set(Inih_LIBRARIES inih CACHE STRING "Iniparser library")
