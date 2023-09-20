#include "board/board.h"
#include "engine/engine.h"
#include <bitset>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

const uint16_t BOOK_MOVES[] = {
    //    movePack(A2, A4, true, false, 0, false, 0) // 1-0 in 21
    movePack(H2, H4, true, false, 0, false, 0) // 0-1 in 32
};
const uint8_t BOOK_MOVE_LEN = sizeof(BOOK_MOVES) / sizeof(uint16_t);

#define PRINT_PGN_ONLY

int main() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Board board = Board();

  Engine engine = Engine(&board);
  // Mate testing
  for (int i = 0; i < 120; i++) {
#ifndef PRINT_PGN_ONLY
    std::cout << "Move " << i << "\n";
#endif
    auto t1 = high_resolution_clock::now();
    bool done = false;
    uint16_t bestMove;
    bestMove = engine.runSearch(2, done);
    if (i <= BOOK_MOVE_LEN - 1) {
      bestMove = BOOK_MOVES[i];
    }
    if (done) {
      break;
    }
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
#ifdef PRINT_PGN_ONLY
    if (i % 2 == 0) {
      if (i % 8 == 0) {
        std::cout << "\n";
      }
      std::cout << (i / 2) + 1 << ". ";
    }
    board.printMove(bestMove);
    std::cout << " ";
#else
    std::cout << ((double)(ms_double.count()) / 1000.0) << " seconds.\nEval: ";
    std::cout << engine.staticEvaluation();
    std::cout << "\nBest move: ";
    board.printMove(bestMove);
    std::cout << "\n";
#endif
    board.makeMove(bestMove);
#ifndef PRINT_PGN_ONLY
    board.printBoard();
#endif
  }
  return 0;
}
