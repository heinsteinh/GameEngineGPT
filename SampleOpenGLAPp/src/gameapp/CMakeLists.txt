set(targetapp ${gameapp}App)

project(${targetapp} C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(.)
include_directories(${INCLUDE_OUTPUT_PATH})

add_executable(${targetapp} main.cpp)
