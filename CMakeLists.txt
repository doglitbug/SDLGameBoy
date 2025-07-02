cmake_minimum_required(VERSION 3.22)
project(SDLGameBoy)

set(CMAKE_CXX_STANDARD 17)

include_directories(src
                    src/gameboy)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/gameboy/cpu.cpp
        src/common.h
        src/engine.cpp
        src/engine.h
        src/gameboy/mmu.cpp
        src/gameboy/mmu.h
        src/gameboy/cartridge.cpp
        src/gameboy/cartridge.h)

target_link_libraries(${PROJECT_NAME} SDL3)
