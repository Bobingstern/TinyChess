#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

float Engine::alphaBeta(float alpha, float beta, uint64_t attackers, int depthleft, int originalDepth){
  if (depthleft == 0) {
    return staticEvaluation();
  }
  uint16_t moves[218];
  this->board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  int total = this->board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
                                         kingAttacks);

  float score = -10000;
  float bestScore = -10000;
  uint16_t bestMove = 0;
  for (int i = 0; i < total; i++) {
    this->board->setAttackers(pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
    uint64_t isolated = this->board->getKing();
    bool check = (attackers & isolated) != 0;
    this->board->makeMove(moves[i]);
    uint64_t a = this->board->getAttackers();
    if (!this->board->isLegal(attackers, check)) {
      this->board->unmakeMove(moves[i]);
      continue;
    }
    // Do stuff here
    score = -alphaBeta(-beta, -alpha, a, depthleft-1, originalDepth);
    this->board->unmakeMove(moves[i]);
    if (depthleft == originalDepth && score > bestScore){
      bestMove = moves[i];
      bestScore = score;
    }
    if( score > alpha )
        alpha = score;
    if( score >= beta ){
        return beta;
    }
  }
  if (depthleft == originalDepth){
    return bestMove;
  }
  return alpha;
}

float Engine::runSearch(int depth) {
  std::cout << "Running search with depth " << depth << "\n";
  board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  uint16_t moves[218];
  board->color = !board->color;
   int total =
       board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
       kingAttacks);
  board->color = !board->color;
  return alphaBeta(-10000, 10000, board->getAttackers(), depth, depth);
}