#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctime>
//position fen rnbqkbnr/ppp1pppp/8/3p4/5P2/8/PPPPP1PP/RNBQKBNR w KQkq - 0 1 moves e2e3 d5d4 e3e4 d4d3 f1d3 h7h5 g1f3 h5h4 e1g1 h4h3 g2h3 e7e6 d3b5 c7c6 b5c4 b7b5 c4e2 b5b4 g1g2 f7f5 e4f5 e6f5 e2c4 g8h6 f3g5 g7g6 g5e6 d8d6 e6f8 h8f8 c2c3 b4c3 d2c3 d6d1 f1d1 c8b7 c1e3 a7a5 e3c5 f8f6 d1e1 e8d7 e1e7 d7c8 b1d2 a5a4 a1d1 g6g5 f4g5 f6g6 c4e6 g6e6 e7e6 a8a5 g5h6 a5c5 h6h7 c8d7 h7h8q d7e6 h8b8 b7a6 b8b6 a6e2 b6c5 e2d1 c5c6 e6e5 d2c4 e5f4 c6d6 f4g5 d6d1 a4a3 b2a3 f5f4 d1g4 g5h6 g4f4 h6g6 h3h4 g6h7 h4h5 h7g7 f4f5 g7g8 h5h6 g8h8 f5g4 h8h7 c4e5 h7h8 h2h3 h8h7 h3h4 h7h8 c3c4 h8h7 a3a4 h7h8 a2a3 h8h7 h4h5 h7h8 c4c5 h8h7 a4a5 h7h8 a3a4 h8h7 c5c6 h7h8 a5a6 h8h7 a4a5 h7h8 c6c7 h8h7 a6a7 h7h8 a5a6 h8h7 c7c8n h7h8 a7a8n h8h7 a6a7 h7h8
float Engine::alphaBeta(float alpha, float beta, uint64_t attackers, int depthleft, int originalDepth, uint16_t &bestMove){
  if (depthleft == 0 || (maxTime != -1 && (std::clock() - startClock) >= maxTime - 5)) {
    float ev = quiesce(alpha, beta, attackers);
    return ev;
  }
  uint16_t moves[218];
  this->board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  int total = this->board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
                                         kingAttacks);
  float score = -10000;
  float bestScore = -10000;
  bool noLegal = true;
  bool shifted = false;
  //board->printBitBoard( board->kingAttacks(board->blackKing, 63 - __builtin_ctzll(board->blackKing) ) );
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
    if (!shifted && depthleft == originalDepth){
      bestMove = moves[i];
    }
    noLegal = false;
    // Do stuff here
    score = -alphaBeta(-beta, -alpha, a, depthleft-1, originalDepth, bestMove);
    //std::cout << "\n";
    this->board->unmakeMove(moves[i]);
    if (depthleft == originalDepth && score > bestScore){
      bestMove = moves[i];
      bestScore = score;
      shifted = true;
    }
    if( score > alpha )
        alpha = score;
    if( score >= beta ){
        return beta;
    }
  }
  if (noLegal && (attackers & board->getKing()) != 0){
    return -10000 + (originalDepth - depthleft); // Crude and dumb
  }
  if (noLegal){
    //std::cout << alpha << "\n";
    return 0;
  }
  
  return alpha;
}

float Engine::quiesce(float alpha, float beta, uint64_t attackers){
  float stand_pat = staticEvaluation();
  if ((maxTime != -1 && (std::clock() - startClock) >= maxTime - 5))
    return stand_pat;
  if( stand_pat >= beta )
    return beta;
  if( alpha < stand_pat )
    alpha = stand_pat;

  uint16_t moves[218];
  this->board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  int total = this->board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
                                         kingAttacks);
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
    uint8_t flag = (0b1111000000000000 & moves[i]) >> 12;
    if (flag != 0b0100){
      this->board->unmakeMove(moves[i]);
      continue;
    }
    float score = -quiesce( -beta, -alpha, a);
    board->unmakeMove(moves[i]);
    if( score >= beta )
      return beta;
    if( score > alpha )
      alpha = score;
  }
  return alpha;
}

uint16_t Engine::runSearch(int depth, int m) {
  startClock = std::clock();
  board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  uint16_t moves[218];
  board->color = !board->color;
  int total =
      board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
  board->color = !board->color;
  
  uint16_t bestMove = 0;
  maxTime = m;
  alphaBeta(-10000, 10000, board->getAttackers(), depth, depth, bestMove);
  return bestMove;
}