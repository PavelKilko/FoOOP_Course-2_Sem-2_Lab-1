cmake_minimum_required(VERSION 3.20)
project(Lab1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "-luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -lstdc++fs -lDwmapi -static")
set(SOURCE_FILES main.cpp)
add_executable(Lab1 ${SOURCE_FILES})
target_link_libraries(Lab1 ${CMAKE_EXE_LINKER_FLAGS})