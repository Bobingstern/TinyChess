#include "engine.h"

Engine::Engine() {
  this->board = NULL;
};

Engine::Engine(Board* board) {
  this->board = board;
}

uint64_t Engine::runPerft(int depth) {
  board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  uint16_t moves[218];
  board->color = !board->color;
  //  int total =
  //      board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
  //      kingAttacks);
  board->color = !board->color;
  return perft(depth, board->getAttackers());
}

uint64_t Engine::perft(int depth, uint64_t attackers) {
  if (depth == 0) {
    return 1ULL;
  }
  uint64_t nodes = 0;
  uint16_t moves[218];

  this->board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  int total = this->board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
                                         kingAttacks);

  for (int i = 0; i < total; i++) {
    this->board->setAttackers(pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
    this->board->makeMove(moves[i]);
    uint64_t a = this->board->getAttackers();
    if (!this->board->isLegal(attackers)) {
      this->board->unmakeMove(moves[i]);
      continue;
    }
    uint64_t newNodes = perft(depth - 1, a);
    nodes += newNodes;
    this->board->unmakeMove(moves[i]);
  }
  return nodes;
}
