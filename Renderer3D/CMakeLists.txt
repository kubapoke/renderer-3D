cmake_minimum_required(VERSION 3.27)
project(Renderer3D)

set(CMAKE_CXX_STANDARD 17)

include_directories(include external)

file(GLOB SOURCES src/*.cpp src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
        ${CMAKE_SOURCE_DIR}/lib/OpenGL32.Lib
)