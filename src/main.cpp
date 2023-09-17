#include "board/board.h"
#include "engine/engine.h"
#include <bitset>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

int main() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Board board = Board();
  board.printBoard();

  Engine engine = Engine(&board);

  for (int i=0; i< 20;i++){
    auto t1 = high_resolution_clock::now();
    float bestMove = engine.runSearch(4);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ((double)(ms_double.count()) / 1000.0) << " seconds. Eval:\n";
    board.printMove(bestMove);
    board.makeMove(bestMove);
    std::cout << "\n";
    board.printBoard();
  }
  return 0;
}
