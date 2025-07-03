get_filename_component(GAME_EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" GAME_EXAMPLE_NAME ${GAME_EXAMPLE_NAME})

project(${GAME_EXAMPLE_NAME} C CXX)

include_directories(.)
include_directories(${INCLUDE_OUTPUT_PATH})
include_directories(${EngineLib_DIR})

add_executable(
  ${GAME_EXAMPLE_NAME} src/main.cpp src/SandboxLayer.cpp src/SandboxLayer.h
                       src/SandboxApp.cpp src/SandboxApp.h)

target_link_libraries(${GAME_EXAMPLE_NAME} EngineLib spdlog::spdlog)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
             PROPERTY VS_STARTUP_PROJECT ${GAME_EXAMPLE_NAME})
