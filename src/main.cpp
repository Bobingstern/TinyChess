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

uint64_t runPerft(Board& board, int depth) {
  board.resetAttackers();
  uint64_t pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks;
  uint16_t moves[218];
  board.color = !board.color;
  int total =
      board.generateMoves(moves, pawnAttacks, rookAttacks, knightAttacks, bishopAttacks, queenAttacks, kingAttacks);
  // board.printBitBoard(board.getAttackers());
  board.color = !board.color;
  return perft(board, depth, board.getAttackers());
}

//---
int main() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Board board = Board();
  board.makeMove(movePack(48, 32, true, false, 0, false, 0));
  board.makeMove(movePack(9, 25, true, false, 0, false, 0));
  board.makeMove(movePack(32, 25, false, false, 0, true, 0));
  board.makeMove(movePack(10, 18, false, false, 0, true, 0));
  //  board.makeMove(movePack(49, 41, false, false, 0, false, 0));
  //  board.makeMove(movePack(9, 17, false, false, 0, false, 0));
  //  board.makeMove(movePack(58, 49, false, false, 0, false, 0));
  //  board.makeMove(movePack(2, 9, false, false, 0, false, 0));
  //  board.makeMove(movePack(57, 42, false, false, 0, false, 0));
  //  board.makeMove(movePack(1, 18, false, false, 0, false, 0));
  //  board.makeMove(movePack(52, 44, false, false, 0, false, 0));
  //  board.makeMove(0b0000101110110110);
  //  board.makeMove(0b0000010110001110);
  //  board.makeMove(0b0000101000111001);
  //  board.makeMove(0b0000010111000110);
  //  board.makeMove(0b0000101001110001);
  //  // board.makeMove(0b0000010100001100);
  //  board.makeMove(0b0000001110000101);
  //  board.makeMove(0b0000110001111010);

  // board.makeMove(movePack(13, 29, true, false, 0, false, 0));
  //  board.makeMove(movePack(55, 47, false, false, 0, false, 0));
  //  board.makeMove(movePack(4, 13, false, false, 0, false, 0));
  //  board.makeMove(movePack(53, 45, false, false, 0, false, 0));

  // board.makeMove(0b0100101000000101);
  // board.makeMove(0b0000100010101000);

  board.printBoard();

  auto t1 = high_resolution_clock::now();

  std::cout << board.color << "\n";

  uint64_t perftNodes = runPerft(board, 5);
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