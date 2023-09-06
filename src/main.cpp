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
uint64_t perft(Board& b, int depth) {

  if (depth == 0) {
    return 1ULL;
  }
  uint16_t moves[218];
  // memset(moves, 0, sizeof(moves));
  // std::cout << b.color << " ";
  int total = b.generateMoves(moves);
  uint64_t nodes = 0;
  // std::cout << b.color << " \n";
  // b.printAllBitBoards();

  for (int i = 0; i < total; i++) {
    // b.printMove(moves[i]);
    //  if (moves[i] == 0)
    //      break;
    // pMove(moves[i]);
    b.makeMove(moves[i]);
    if (!b.isLegal()) {
      b.unmakeMove(moves[i]);
      continue;
    }
    // b.printBoard();
    // std::cout << "\n";
    uint64_t newNodes = perft(b, depth - 1);
    nodes += newNodes;
    // if (depth == 5){
    //     //b.printMove(moves[i]);
    //     // pMove(moves[i]);
    //     // std::cout << ": " << newNodes << "\n";
    // }
    b.unmakeMove(moves[i]);
  }
  return nodes;
  // static uint16_t moves[218];
  // memset(moves, 0, sizeof(moves));
  // //std::cout << b.color << "\n";
  // b.generateMoves(moves);
  // for (int i=0;i<218;i++){
  //     //b.printMove(moves[i]);
  //     if (moves[i] == 0)
  //         break;
  //     b.makeMove(moves[i]);
  //     //b.printBoard();
  //     b.unmakeMove(moves[i]);
  //     //std::cout << "\n";
  //     //board.printBoard();
  // }
  // return 0;
}

int main() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Board board = Board();
  // board.makeMove(0b0000101101110101);
  // board.makeMove(0b0000011111001111);
  // board.makeMove(0b0000110101111100);
  // board.makeMove(0b0000100111011111);
  board.printBoard();

  auto t1 = high_resolution_clock::now();
  uint64_t perftNodes = perft(board, 5);
  auto t2 = high_resolution_clock::now();
  /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = t2 - t1;

  std::cout << "Nodes:" << perftNodes << " in " << ms_double.count() << "ms\n";
  std::cout << (double)(perftNodes) / ((double)(ms_double.count()) / 1000.0) << " NPS";
  // board.printAllBitBoards();
  //  uint16_t moves[218];
  //  //memset(moves, 0, sizeof(moves));
  //  int total = board.generateMoves(moves);
  //  for (int i=0;i<total;i++){
  //      //board.printMove(moves[i]);
  //      if (moves[i] == 0)
  //          break;
  //      board.makeMove(moves[i]);
  //      if (!board.isLegal()){
  //          board.unmakeMove(moves[i]);
  //          continue;
  //      }
  //      board.printBoard();
  //      board.unmakeMove(moves[i]);
  //      std::cout << "\n";
  //      //board.printBoard();
  //  }

  return 0;
}