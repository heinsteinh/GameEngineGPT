set(appName "AppExample")

add_executable(${appName} main.cpp)

target_link_libraries(${appName} PUBLIC EngineLib)

include_directories(${INCLUDE_OUTPUT_PATH})
link_directories(${LIBRARY_OUTPUT_PATH})

if(APPLE)
  target_link_libraries(${appName} PUBLIC ${SDL2_LIBRARIES})
  target_link_libraries(${appName} PUBLIC ${SDL2_LIBRARIES})
elseif(WIN32)
  target_link_libraries(${appName} PRIVATE SDL2::SDL2)
  target_link_libraries(${appName} PRIVATE SDL2::SDL2main)
else()
  target_link_libraries(${appName} PRIVATE ${SDL2_LIBRARIES})
  # target_link_libraries(${appName}   PUBLIC SDL2 glew)
endif()

target_include_directories(
  ${appName}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
         # Include configured header file which contains engine metadata
         ${CMAKE_CURRENT_BINARY_DIR}/include/ ${EngineLib_DIR})
