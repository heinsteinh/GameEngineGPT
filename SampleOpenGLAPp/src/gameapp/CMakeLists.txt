set(targetapp ${gameapp}App)

project(${targetapp} C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(.)
include_directories(${INCLUDE_OUTPUT_PATH})

# Collect sources into the variable Engine_Framework_CmdGraphic_Common_Src
file(
  GLOB_RECURSE
  EngineCore_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c??")

add_executable(${targetapp} ${EngineCore_SRC} main.cpp)

# Include directories
target_include_directories(
  ${targetapp} PUBLIC ${glew_SOURCE_DIR}/include ${glfw_SOURCE_DIR}/include
                      ${sdl2_SOURCE_DIR}/include)
