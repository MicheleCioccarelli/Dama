cmake_minimum_required(VERSION 3.19)
project(MakeDamaGreatAgain)

set(CMAKE_CXX_STANDARD 14)

add_executable(MakeDamaGreatAgain main.cpp board/board.cpp board/board.h helpers/helpers.h helpers/helpers.cpp renders/render.h renders/render.cpp)