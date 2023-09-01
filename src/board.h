#pragma once

#include <stdint.h>
#include <vector>
#include <stdlib.h>


#define bitRead(value, bit) (((value) >> (63 - bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (63 - bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (63 - bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, 63 - bit) : bitClear(value, 63 - bit))

// 00 00 000000 000000
// 2 bits unused
// 2 bits r for promo
// 6 r to index
// 6 r from index
#define moveReadFromIndex(move) (move & 0b111111)
#define moveReadToIndex(move) (move & 0b111111000000)
#define moveReadPromotion(move) (move & 0b11000000000000)
void moveSet(uint16_t& move, uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture,
             uint8_t castle);
uint16_t movePack(uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture,
                  uint8_t castle);

class Board {
  public:
    Board();

    void printBoard();
    void printBitBoard(uint64_t bb);
    void printAllBitBoards();

    // Move generator
    void makeMove(uint16_t a);
    void unmakeMove(uint16_t a);
    // History
    std::vector<uint8_t> captures;
    //Utilities
    uint64_t whiteOccupation();
    uint64_t blackOccupation();
    uint64_t combinedOccupation();
    uint64_t bitReverse(uint64_t n);
    uint64_t flipVertical(uint64_t n);
    uint64_t rotate90(uint64_t n);
    uint64_t rotateAnti90(uint64_t n);

    uint64_t obstructionDifference(uint8_t from, uint64_t ray);
    uint64_t fileAttacks(uint8_t from);
    uint64_t rankAttacks(uint8_t from);
    uint64_t rookAttacks(uint8_t from);
    uint64_t bishopAttacks(uint8_t from);
    uint64_t knightAttacks(uint64_t isolated, uint8_t from);
    uint64_t kingAttacks(uint64_t isolated, uint8_t from);
    void printMove(uint16_t a);


    

    std::vector<uint16_t> pawnMoves(bool color);
    std::vector<uint16_t> rookMoves(bool color);
    std::vector<uint16_t> bishopMoves(bool color);
    std::vector<uint16_t> queenMoves(bool color);
    std::vector<uint16_t> knightMoves(bool color);
    std::vector<uint16_t> kingMoves(bool color);

    uint64_t whitePawns;
    uint64_t blackPawns;
    uint64_t whiteKnights;
    uint64_t blackKnights;
    uint64_t whiteBishops;
    uint64_t blackBishops;
    uint64_t whiteRooks;
    uint64_t blackRooks;
    uint64_t whiteQueens;
    uint64_t blackQueens;
    uint64_t whiteKing;
    uint64_t blackKing;
    uint64_t** bitboards = new uint64_t*[12];

};