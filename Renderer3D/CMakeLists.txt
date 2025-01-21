cmake_minimum_required(VERSION 3.27)
project(Renderer3D)

set(CMAKE_CXX_STANDARD 17)

include_directories(include external)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
        ${CMAKE_SOURCE_DIR}/lib/OpenGL32.Lib
)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(${PROJECT_NAME} copy_assets)