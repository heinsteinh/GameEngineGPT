# Download dependencies by using FetchContent_Declare Use
# FetchContent_MakeAvailable only in those code parts where the dependency is
# actually needed

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.0.1
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  FIND_PACKAGE_ARGS 11.0.1)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
  GIT_SHALLOW ON
  GIT_PROGRESS ON)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.15.0
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  FIND_PACKAGE_ARGS 1.15.0)
FetchContent_MakeAvailable(gtest)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.9
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  FIND_PACKAGE_ARGS 1.90.9)
FetchContent_MakeAvailable(imgui)
add_library(
  imgui
  ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4
  GIT_PROGRESS ON)
FetchContent_MakeAvailable(stb)

set(SPDLOG_FMT_EXTERNAL
    ON
    CACHE BOOL "" FORCE)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  FIND_PACKAGE_ARGS 1.14.1)

FetchContent_MakeAvailable(spdlog)
# ############################# SDL2
# ##############################################################################

# Configure SDL
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_INSTALL
    OFF
    CACHE BOOL "Disable SDL installation" FORCE)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.x
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SDL2)

# ############################# glew
# ##############################################################################
set(glew-cmake_BUILD_SHARED
    OFF
    CACHE BOOL "Disable shared build of glew" FORCE)

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG glew-cmake-2.2.0)
FetchContent_MakeAvailable(glew)
