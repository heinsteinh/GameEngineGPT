cmake_minimum_required(VERSION 3.24)

project(app-renderer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Build type: None Debug Release RelWithDebInfo MinSizeRel"
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

option(FTS_DEBUG "Enable FTS_DEBUG" on)
option(FTS_IMGUI "Enable FTS_IMGUI" on)

# Options
option(FTS_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(FTS_BUILD_DOC "Build documentation" OFF)
option(FTS_BUILD_EXAMPLE "Build example" ON)
option(FTS_BUILD_TESTS "Build tests" OFF)
option(FTS_FORCE_COLORED_OUTPUT
       "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

message(STATUS "FTS_BUILD_BENCHMARKS = ${FTS_BUILD_BENCHMARKS}")
message(STATUS "FTS_BUILD_DOC = ${FTS_BUILD_DOC}")
message(STATUS "FTS_BUILD_EXAMPLE = ${FTS_BUILD_EXAMPLE}")
message(STATUS "FTS_BUILD_TESTS= ${FTS_BUILD_TESTS}")

message(STATUS "CMAKE_VERSION = ${CMAKE_VERSION}")
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS "C Compiler executable: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler executable: ${CMAKE_CXX_COMPILER}")
message(STATUS "Linker executable: ${CMAKE_LINKER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# -- config output directories
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Host/bin)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Host/lib)
  set(INCLUDE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Host/include)

else()
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Host/bin)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Host/lib)
  set(INCLUDE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Host/include)
endif()

# Call configure files on ver.h.in to set the version. Uses the standard
# ${VARIABLE} syntax in the file
configure_file("${CMAKE_SOURCE_DIR}/cmake/inConfig/VersionEngine.h.in"
               ${INCLUDE_OUTPUT_PATH}/VersionEngine.h)

# configure the path.h.in file. This file can only use the @VARIABLE@ syntax in
# the file
configure_file("${CMAKE_SOURCE_DIR}/cmake/inConfig/PathEngine.h.in"
               ${INCLUDE_OUTPUT_PATH}/PathEngine.h @ONLY)

# configuration
configure_file("${CMAKE_SOURCE_DIR}/cmake/inConfig/config.h.in"
               ${INCLUDE_OUTPUT_PATH}/Config.h)

message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")

# Download dependencies through CMake
include(cmake/dependencies.cmake)

set(EngineLib_DIR ${PROJECT_SOURCE_DIR}/enginelib)

add_subdirectory(enginelib)

if(FTS_BUILD_EXAMPLE)
  # add_subdirectory(app-example)
  add_subdirectory(sandbox)
endif()

if(FTS_BUILD_TESTS)
  add_subdirectory(tests)
endif()
