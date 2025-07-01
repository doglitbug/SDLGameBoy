cmake_minimum_required(VERSION 3.22)
project(SDLGameBoy)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/cpu.cpp
        src/common.h
        src/engine.cpp
        src/engine.h
        src/mmu.cpp
        src/mmu.h)

target_link_libraries(${PROJECT_NAME} SDL3)
