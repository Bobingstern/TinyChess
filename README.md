# TinyChess

A small chess engine in C++.

## Install

These steps assume Windows.

1. Install MinGW and Make or Ninja.*
2. Install CMake.
3. Clone this repo.
4. `mkdir build && cd build` in project root.
5. `cmake .. -G "MinGW Makefiles"` or `cmake .. -G "Ninja"`.*
6. `cmake --build .`

\* You could probably use other generators that end up using `gcc`/`g++` or `clang`, but that is untested. Using MSVC
will _not_ work.

`g++ src/*.cpp src/board/*.cpp src/board/pieces/*.cpp -o main.exe` seems to work without issues on Windows

## Usage

Run `main.exe`.
