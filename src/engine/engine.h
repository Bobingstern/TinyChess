#pragma once

#include <stdint.h>
#include <vector>
#include <stdlib.h>
#include "../board/board.h"


class Engine {
  public:
    Engine();
    Engine(Board* board);

    uint64_t runPerft(int depth);
    
    Board* board = NULL;

  private:
    uint64_t perft(int depth, uint64_t attackers);
};
