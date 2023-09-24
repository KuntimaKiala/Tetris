cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)


project(Tetris) 


file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(Tetris ${SRC_FILES})

target_link_directories(Tetris PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Tetris PUBLIC ncurses)