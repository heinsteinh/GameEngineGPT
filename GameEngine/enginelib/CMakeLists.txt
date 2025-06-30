set(LibName "EngineLib")

project(${LibName} C CXX)

include_directories(.)
include_directories(${INCLUDE_OUTPUT_PATH})
include_directories(${EngineLib_DIR})

# Collect sources into the variable Engine_Framework_CmdGraphic_Common_Src file
# (GLOB_RECURSE EngineLib_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.txt"
# "${CMAKE_CURRENT_SOURCE_DIR}/*.*"
# "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
# "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/*.c??")

# Collect sources into the variable Engine_Core_Src
file(
  GLOB_RECURSE
  Engine_Core_Src
  "${EngineLib_DIR}/Core/*.txt"
  "${EngineLib_DIR}/Core/*.h"
  "${EngineLib_DIR}/Core/*.inl"
  "${EngineLib_DIR}/Core/*.hpp"
  "${EngineLib_DIR}/Core/*.cpp")

file(
  GLOB_RECURSE
  Engine_Utils_Src
  "${EngineLib_DIR}/Utils/*.txt"
  "${EngineLib_DIR}/Utils/*.h"
  "${EngineLib_DIR}/Utils/*.inl"
  "${EngineLib_DIR}/Utils/*.hpp"
  "${EngineLib_DIR}/Utils/*.cpp")

add_library(${LibName} ${Engine_Core_Src} ${Engine_Utils_Src})

target_compile_definitions(
  ${LibName} PUBLIC GLM_ENABLE_EXPERIMENTAL # GLFW_INCLUDE_VULKAN
                    GLM_FORCE_DEPTH_ZERO_TO_ONE GLM_FORCE_RADIANS)

target_include_directories(
  ${LibName}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
         # Include configured header file which contains engine metadata
         ${CMAKE_CURRENT_BINARY_DIR}/include/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Include directories
target_include_directories(
  ${LibName} PUBLIC ${glew_SOURCE_DIR}/include ${glfw_SOURCE_DIR}/include
                    ${sdl2_SOURCE_DIR}/include)

target_link_libraries(
  ${LibName}
  PUBLIC fmt::fmt
         glm::glm
         imgui
         libglew_static
         spdlog::spdlog_header_only
         SDL2::SDL2main
         SDL2::SDL2-static)
# Correctly locate SDL2.dll and set up the copy command for Windows
if(WIN32)
  # Copy the SDL2.dll file to the executable directory after build
  add_custom_command(
    TARGET ${LibName}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:${LibName}>)
endif()

message(STATUS "sdl2: ${sdl2_SOURCE_DIR}")
message(STATUS "glew: ${glew_SOURCE_DIR}")
message(STATUS "glfw: ${glfw_SOURCE_DIR}")
message(STATUS "spdlog: ${spdlog_SOURCE_DIR}")
