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
  board.makeMove(movePack(16, 24, false, false, 0, false, 0));
  board.printBoard();
  
  Engine engine = Engine(&board);
  // Mate testing
  for (int i=0;i<120;i++){
    auto t1 = high_resolution_clock::now();
    uint16_t bestMove;
    bool done = false;
    bestMove = engine.runSearch(2, done);
    if (done){
      break;
    }
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ((double)(ms_double.count()) / 1000.0) << " seconds. Eval:\n";
    board.printMove(bestMove);
    board.makeMove(bestMove);
    board.printBoard();
  }
  return 0;
}
