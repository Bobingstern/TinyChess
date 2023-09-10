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

  auto t1 = high_resolution_clock::now();
  uint64_t perftNodes = engine.runPerft(2);
  auto t2 = high_resolution_clock::now();
  // /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "Generated " << perftNodes << " nodes in " << ms_double.count() << "ms\n";
  std::cout << (double)(perftNodes) / ((double)(ms_double.count()) / 1000.0) << " NPS";

  return 0;
}
