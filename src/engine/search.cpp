#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

float Engine::alphaBeta(float alpha, float beta, uint64_t attackers, int depthleft, int originalDepth,
                        uint16_t& bestMove) {
  if (depthleft == 0) {
    float ev = quiesce(alpha, beta, attackers, 4);
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
    noLegal = false;
    // Do stuff here
    score = -alphaBeta(-beta, -alpha, a, depthleft - 1, originalDepth, bestMove);
    this->board->unmakeMove(moves[i]);
    if (depthleft == originalDepth && score > bestScore) {
      bestMove = moves[i];
      bestScore = score;
    }
    if (score > alpha)
      alpha = score;
    if (score >= beta) {
      return beta;
    }
  }
  if (noLegal && depthleft == originalDepth) {
    return -1000000; // Crude and dumb
  }
  if (noLegal)
    return -100000;

  return alpha;
}

bool Engine::badCapture(uint16_t move) {
  int prevMover = board->previousMover[board->depth - 1];
  int prevCapture = board->captures[board->currentCapture - 1];
  uint64_t to = 1ULL << (63 - ((move & 0b0000111111000000) >> 6));
  if (prevMover == BB_WHITE_PAWNS || prevMover == BB_BLACK_PAWNS) {
    return false;
  }
  // If piece being captures is defended by a pawn
  if (board->color == BLACK) {
    // White just made a capture
    uint64_t possiblePawns = to << 9;
    possiblePawns |= to << 7;
    if ((board->blackPawns & possiblePawns) != 0) {
      // Defended by a pawn
      if (prevMover == BB_WHITE_KNIGHTS || prevMover == BB_WHITE_BISHOPS) {
        if (prevCapture == BB_BLACK_PAWNS) {
          return true;
        }
      }
    }
  }
  if (board->color == WHITE) {
    // White just made a capture
    uint64_t possiblePawns = to >> 9;
    possiblePawns |= to >> 7;
    if ((board->whitePawns & possiblePawns) != 0) {
      // Defended by a pawn
      if (prevMover == BB_BLACK_KNIGHTS || prevMover == BB_BLACK_BISHOPS) {
        if (prevCapture == BB_WHITE_PAWNS) {
          return true;
        }
      }
    }
  }
  return false;
}

float Engine::quiesce(float alpha, float beta, uint64_t attackers, int depthleft) {

  uint16_t moves[218];
  this->board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  int total = this->board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
                                         kingAttacks);

  float standPat = staticEvaluation();
  standPat += hammingWeight(pawnAttacks | rookAttacks | knightAttacks | bishopAttacks | queenAttacks | kingAttacks);
  if (standPat >= beta) {
    return beta;
  }
  if (alpha < standPat) {
    alpha = standPat;
  }
  if (depthleft == 0) {
    return standPat;
  }

  float score = -10000;
  float bestScore = -10000;
  bool noLegal = true;
  for (int i = 0; i < total; i++) {
    this->board->setAttackers(pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
    uint64_t isolated = this->board->getKing();
    bool check = (attackers & isolated) != 0;
    this->board->makeMove(moves[i]);
    uint64_t a = this->board->getAttackers();
    uint8_t flag = (moves[i] >> 12);
    if (!this->board->isLegal(attackers, check)) {
      this->board->unmakeMove(moves[i]);
      continue;
    }
    noLegal = false;
    // Not a capture
    if ((flag != 0b0100 && flag != 0b0101 && !(flag >= 0b1100))) {
      this->board->unmakeMove(moves[i]);
      continue;
    }
    // Bad capture
    if (badCapture(moves[i])) {
      this->board->unmakeMove(moves[i]);
      continue;
    }
    // Do stuff here
    score = -quiesce(-beta, -alpha, a, depthleft - 1);
    this->board->unmakeMove(moves[i]);

    if (score >= beta) {
      return beta;
    }
    int bigDelta = 975;
    if (flag >= 0b1100) {
      bigDelta += 775;
    }
    if (score < alpha - bigDelta) {
      return alpha;
    }
    if (score > alpha)
      alpha = score;
  }

  return alpha;
}

uint16_t Engine::runSearch(int depth, bool& done) {
  board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  uint16_t moves[218];
  board->color = !board->color;
  int total =
      board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
  board->color = !board->color;
  uint16_t bestMove = 0;
  if (alphaBeta(-10000, 10000, board->getAttackers(), depth, depth, bestMove) == -1000000) {
    done = true;
  }
  return bestMove;
}