#pragma once

#include "../board/board.h"
#include <stdint.h>
#include <stdlib.h>
#include <vector>

class Engine {
  public:
    Engine();
    explicit Engine(Board* board);

    uint64_t runPerft(int depth);

    Board* board = nullptr;

  private:
    uint64_t perft(int depth, uint64_t attackers);
};
