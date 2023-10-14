# TinyChess

UCI chess engine designed to run with low RAM

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

Run `main.exe`. The engine supports basic UCI commands.

### Testing

#### Manually running a Cutechess tournament

Compile different versions of TinyChess, (you can use the same version, but it will be quite boring) and add them as
engines to Cutechess. (using the GUI is the easiest) You can then set up a tournament via the GUI or use the command
line.

Example command to start a SPRT with 50 games with a time control of 60 seconds and with engine configurations
`TinyChess0` and `TinyChess1` and using the Silver opening suite and concurrency of 5.

```cmd
cutechess-cli -engine conf=TinyChess1 -engine conf=TinyChess0 -each proto=uci tc=60 timemargin=100 -sprt elo0=0 elo1=5 alpha=0.05 beta=0.05 -games 50 -openings file=Silver_Suite.pgn format=pgn plies=5 -concurrency 5
```

#### TinyChessTest

[TinyChessTest](https://github.com/UnsignedArduino/TinyChessTest) is a small testing program, visit it's repository 
to find out how to set up the tester and run a tournament. This will be deprecated in favor of TinyTest.

#### TinyTest

[TinyTest](https://tinytest.vercel.app/) is the distributed testing program for TinyChess, under development. 
