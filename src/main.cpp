#include "board/board.h"
#include <bitset>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

void numToLetter(uint8_t a) {
  if (a % 8 == 0) {
    std::cout << "a";
  }
  if (a % 8 == 1) {
    std::cout << "b";
  }
  if (a % 8 == 2) {
    std::cout << "c";
  }
  if (a % 8 == 3) {
    std::cout << "d";
  }
  if (a % 8 == 4) {
    std::cout << "e";
  }
  if (a % 8 == 5) {
    std::cout << "f";
  }
  if (a % 8 == 6) {
    std::cout << "g";
  }
  if (a % 8 == 7) {
    std::cout << "h";
  }
}
void pMove(uint16_t a) {
  uint8_t from = (a & 0b0000000000111111);
  uint8_t to = ((a >> 6) & 0b0000000000111111);
  numToLetter(from);
  std::cout << (int)(((63 - from) / 8 + 1));
  numToLetter(to);
  std::cout << (int)((63 - to) / 8 + 1);
}
// g++ src/*.cpp src/pieces/*.cpp -o main.exe -std=c++17
uint64_t perft(Board& b, int depth, uint64_t attackers) {

  if (depth == 0) {
    return 1ULL;
  }
  uint64_t nodes = 0;
  uint16_t moves[218];

  b.resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  int total = b.generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);

  for (int i = 0; i < total; i++) {
    b.setAttackers(pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
    b.makeMove(moves[i]);
    uint64_t a = b.getAttackers();
    if (!b.isLegal(attackers)) {
      b.unmakeMove(moves[i]);
      continue;
    }
    uint64_t newNodes = perft(b, depth - 1, a);
    nodes += newNodes;
    if (depth == 5) {
      // b.printMove(moves[i]);
      pMove(moves[i]);
      std::cout << ": " << newNodes << "\n";
    }
    b.unmakeMove(moves[i]);
  }
  return nodes;
}

//---
int main() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Board board = Board();
  board.makeMove(0b0000101110110110);
  // board.makeMove(0b0000010110001110);
  // board.makeMove(0b0000101111111110);
  // board.makeMove(0b0000010111000101);
  // board.makeMove(0b0000101011110011);

  board.printBoard();

  auto t1 = high_resolution_clock::now();

  std::cout << board.color << "\n";
  board.resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  uint16_t moves[218];
  int total =
      board.generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
  uint64_t perftNodes = perft(board, 5, board.getAttackers());
  // //Testing obstruction difference
  // // uint64_t a;
  // // for (int i=0;i<100000;i++)
  // //     a = board.rookAttacks(i % 64) | board.bishopAttacks(i % 64);
  // //board.printBitBoard(a);
  auto t2 = high_resolution_clock::now();
  // /* Getting number of milliseconds as a double. */

  duration<double, std::milli> ms_double = t2 - t1;
  // //std::cout << (double)100000 / ((double)(ms_double.count())) << " Calculations per ms\n";
  std::cout << "Nodes:" << perftNodes << " in " << ms_double.count() << "ms\n";
  std::cout << (double)(perftNodes) / ((double)(ms_double.count()) / 1000.0) << " NPS";

  return 0;
}