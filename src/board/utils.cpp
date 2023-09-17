#include "board.h"
#include <bitset>
#include <iostream>
#include <vector>

uint64_t Board::whiteOccupation() {
  return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
}

uint64_t Board::blackOccupation() {
  return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
}

uint64_t Board::combinedOccupation() {
  return whiteOccupation() | blackOccupation();
}

int Board::hammingWeight(uint64_t x) {
  x -= (x >> 1) & m1;             // put count of each 2 bits into those 2 bits
  x = (x & m2) + ((x >> 2) & m2); // put count of each 4 bits into those 4 bits
  x = (x + (x >> 4)) & m4;        // put count of each 8 bits into those 8 bits
  return (x * h01) >> 56;         // returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24)
}

int Board::pawnsCount(bool color){
  return hammingWeight(color == WHITE ? whitePawns : blackPawns);
}
int Board::knightsCount(bool color){
  return hammingWeight(color == WHITE ? whiteKnights : blackKnights);
}
int Board::bishopsCount(bool color){
  return hammingWeight(color == WHITE ? whiteBishops : blackBishops);
}
int Board::rooksCount(bool color){
  return hammingWeight(color == WHITE ? whiteRooks : blackRooks);
}
int Board::queensCount(bool color){
  return hammingWeight(color == WHITE ? whiteQueens : blackQueens);
}


void printNumToLetter(uint8_t a) {
  if (a % 8 == 0) {
    std::cout << "a";
  } else if (a % 8 == 1) {
    std::cout << "b";
  } else if (a % 8 == 2) {
    std::cout << "c";
  } else if (a % 8 == 3) {
    std::cout << "d";
  } else if (a % 8 == 4) {
    std::cout << "e";
  } else if (a % 8 == 5) {
    std::cout << "f";
  } else if (a % 8 == 6) {
    std::cout << "g";
  } else if (a % 8 == 7) {
    std::cout << "h";
  }
}
void pMove(uint16_t a) {
  uint8_t from = (a & 0b0000000000111111);
  uint8_t to = ((a >> 6) & 0b0000000000111111);
  printNumToLetter(from);
  std::cout << (int)(((63 - from) / 8 + 1));
  printNumToLetter(to);
  std::cout << (int)((63 - to) / 8 + 1);
}

void Board::printMove(uint16_t a, bool bin) {
  if (bin) {
    std::bitset<16> m = a;
    // std::bitset<4> flag = a >> 12;
    std::cout << "Binary Move:" << m << "\n";
    std::cout << "From:" << moveReadFromIndex(a) << "\n";
    std::cout << "To:" << (moveReadToIndex(a) >> 6) << "\n";
    std::cout << "Flag:" << moveReadPromotion(a) << "\n";
  } else {
    uint8_t from = (a & 0b0000000000111111);
    uint8_t to = ((a >> 6) & 0b0000000000111111);
    printNumToLetter(from);
    std::cout << (int)(((63 - from) / 8 + 1));
    printNumToLetter(to);
    std::cout << (int)((63 - to) / 8 + 1);
  }
}

uint64_t Board::bitReverse(uint64_t b) {
  b = (b & 0x5555555555555555) << 1 | ((b >> 1) & 0x5555555555555555);
  b = (b & 0x3333333333333333) << 2 | ((b >> 2) & 0x3333333333333333);
  b = (b & 0x0f0f0f0f0f0f0f0f) << 4 | ((b >> 4) & 0x0f0f0f0f0f0f0f0f);
  b = (b & 0x00ff00ff00ff00ff) << 8 | ((b >> 8) & 0x00ff00ff00ff00ff);

  return (b << 48) | ((b & 0xffff0000) << 16) | ((b >> 16) & 0xffff0000) | (b >> 48);
}




// Hyperbola Quintessence for Sliding moves
// uint64_t Board::fileAttacks(uint64_t bitMask, uint8_t from){
//     uint64_t forward, reverse;
//     uint64_t fileMaskEx = (0x101010101010101ULL << (uint16_t)(7 - (from % 8))) & (~bitMask);
//     forward = combinedOccupation() & fileMaskEx;
//     reverse = bitReverse(forward);
//     forward -= bitMask;
//     reverse -= bitReverse(bitMask);
//     forward ^= bitReverse(reverse);
//     forward &= fileMaskEx;
//     return forward;
// }
// //Works ig
// uint64_t Board::rankAttacks(uint64_t s, uint8_t from){
//     uint64_t rank = (0xFFULL << ( 8*(7 - (uint8_t)(from / 8) )));
//     uint64_t o = (combinedOccupation() & rank);
//     uint64_t oPrime = bitReverse(o);
//     uint64_t sPrime = bitReverse(s);
//     uint64_t positiveRayAttacks = o ^ (o - 2*s);
//     uint64_t negativeRayAttacks = bitReverse(oPrime ^ (oPrime - 2*sPrime));
//     uint64_t lineAttacks = (o - 2*s) ^ bitReverse(oPrime - 2*sPrime);
//     return lineAttacks & rank;

// }

// Obstruction Difference

uint64_t Board::obstructionDifference(uint8_t from, uint64_t ray) {
  uint64_t lower = (ray ^ (1ull << (63 - from))) & ((1ull << (63 - from)) - 1);
  uint64_t upper = (ray ^ (1ull << (63 - from))) & (0xFFFFFFFFFFFFFFFF << (63 - from));
  uint64_t linemaskEx = lower | upper;
  lower &= combinedOccupation();
  upper &= combinedOccupation();
  uint64_t ms1B = 0x8000000000000000ULL >> __builtin_clzll(lower | 1);
  uint64_t odiff = upper ^ (upper - ms1B);
  return odiff & linemaskEx;
}

uint64_t Board::attackHQ(uint64_t mask, uint64_t isolated, uint8_t from) {
  uint64_t o = combinedOccupation() & mask;
  uint64_t r = bitReverse(o);
  return ((o - isolated) ^ bitReverse(r - (1ULL << (from ^ 56)))) & mask;
}
