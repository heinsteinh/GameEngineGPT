set(AppName "Game")

project(${AppName} C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
# configure_file("${CMAKE_SOURCE_DIR}/cmake/inConfig/VersionEngine.h.in"
# ${INCLUDE_OUTPUT_PATH}/VersionEngine.h)

# configure the path.h.in file. This file can only use the @VARIABLE@ syntax in
# the file
configure_file("${CMAKE_SOURCE_DIR}/../cmake/InConfig/PathEngine.h.in"
               ${INCLUDE_OUTPUT_PATH}/PathEngine.h @ONLY)

# configuration
configure_file("${CMAKE_SOURCE_DIR}/../cmake/InConfig/Config.h.in"
               ${INCLUDE_OUTPUT_PATH}/Config.h)

message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")

# Download dependencies through CMake
include("${CMAKE_SOURCE_DIR}/../cmake/dependencies.cmake")

set(EngineLib_DIR ${PROJECT_SOURCE_DIR}/gameapp)

add_subdirectory(gameapp)
