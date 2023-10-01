// https://analog-hors.github.io/webperft/

#include "engine.h"
#include <iostream>
#include <chrono>


uint64_t Engine::runPerft(int depth) {
  std::cout << "Running perft with depth " << depth << "\n";
  board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  uint16_t moves[218];
  board->color = !board->color;
   int total =
       board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
       kingAttacks);
  board->color = !board->color;
  auto t1 = std::chrono::high_resolution_clock::now();
  int result =  perft(depth, board->getAttackers(), depth);
  std::cout << "Total nodes: " << result << "\n";
  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> ms_double = t2 - t1;
  std::cout << result / ((double)(ms_double.count()) / 1000.0) << " nps \n";
  return result;
}

uint64_t Engine::perft(int depth, uint64_t attackers, int originalDepth) {
  if (depth == 0) {
    return 1ULL;
  }
  uint64_t nodes = 0;
  uint16_t moves[218];

  this->board->resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  int total = this->board->generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks,
                                         kingAttacks);
  int currentPiece = 0; //Pawns
  int currentVictim = 4;
  bool onCaps = true;
  uint64_t pieceCpy = board->color == WHITE ? board->whitePawns : board->blackPawns;
  uint64_t isolatedPiece = pieceCpy & ((~pieceCpy) + 1);
  uint64_t moveSet = board->generatePawnAttacks(isolatedPiece) & (board->color == WHITE ? board->blackOccupation() : board->whiteOccupation());
  //& (*board->bitboards[board->color == WHITE ? currentVictim + 6 : currentVictim]);
  uint64_t standardCpy = moveSet;
  // Staged Move Gen
  int i = 0;
  while (pieceCpy != 0) {
    // Generate the move
    uint16_t move = 0;
    // ---------------
    this->board->setAttackers(pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
    uint64_t isolated = this->board->getKing();
    bool check = (attackers & isolated) != 0;
    
    this->board->makeMove(move);
    board->printBoard();
    uint64_t a = this->board->getAttackers();
    if (!this->board->isLegal(attackers, check)) {
      this->board->unmakeMove(move);
      continue;
    }
    uint64_t newNodes = perft(depth - 1, a, originalDepth);
    if (depth == originalDepth) {
      std::cout << "(" << (i + 1) << "/" << total << ") ";
      this->board->printMove(move);
      std::cout << ": " << newNodes << " nodes\n";
    }
    nodes += newNodes;
    this->board->unmakeMove(move);
    i++;
  }
  return nodes;
}
