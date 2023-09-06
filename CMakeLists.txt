cmake_minimum_required(VERSION 3.22)
project("TinyChess")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_TANDARD_REQUIRED True)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")

add_executable(main ${sources})
