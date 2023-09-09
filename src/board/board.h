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
    void sliceReadd();
    // History
    int captures[32];
    int currentCapture = 0;
    int depth = 0;
    uint16_t previousMoves[12];
    uint16_t previousMover[12];
    int caps = 0;
    
    bool color; // 0 == White
    //Utilities
    uint64_t whiteOccupation();
    uint64_t blackOccupation();
    uint64_t combinedOccupation();
    uint64_t bitReverse(uint64_t n);
    uint64_t flipVertical(uint64_t n);
    uint64_t rotate90(uint64_t n);
    uint64_t rotateAnti90(uint64_t n);

    uint64_t obstructionDifference(uint8_t from, uint64_t ray);
    uint64_t attackHQ(uint64_t mask, uint64_t isolated, uint8_t from);

    uint64_t fileAttacks(uint8_t from);
    uint64_t rankAttacks(uint8_t from);
    uint64_t rookAttacks(uint8_t from);
    uint64_t bishopAttacks(uint8_t from);
    uint64_t knightAttacks(uint64_t isolated, uint8_t from);
    uint64_t kingAttacks(uint64_t isolated, uint8_t from);
    int hammingWeight(uint64_t a);

    uint64_t pawnAttackers;
    uint64_t rookAttackers;
    uint64_t knightAttackers;
    uint64_t bishopAttackers;
    uint64_t queenAttackers;
    uint64_t kingAttackers;

    // Castling flags
    bool flagWhiteKingsideCastle = 1; // 1 means it can castle
    int wkscDepth = -1; // White king sidecastle trip depth

    bool flagWhiteQueensideCastle = 1;
    int wqscDepth = -1;

    bool flagBlackKingsideCastle = 1;
    int bkscDepth = -1;

    bool flagBlackQueensideCastle = 1;
    int bqscDepth = -1;

    //Magic not bb
    uint64_t m1  = 0x5555555555555555; //binary: 0101...
    uint64_t m2  = 0x3333333333333333; //binary: 00110011..
    uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
    uint64_t m8  = 0x00ff00ff00ff00ff; //binary:  8 zeros,  8 ones ...
    uint64_t m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
    uint64_t m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
    uint64_t h01 = 0x0101010101010101;


    void printMove(uint16_t a);

    int pawnMoves(bool color, uint16_t* moves, int i);
    int rookMoves(bool color, uint16_t* moves, int i);
    int bishopMoves(bool color, uint16_t* moves, int i);
    int queenMoves(bool color, uint16_t* moves, int i);
    int knightMoves(bool color, uint16_t* moves, int i);
    int kingMoves(bool color, uint16_t* moves, int i);

    int generateMoves(uint16_t* moves, uint64_t &pawnAttacks, uint64_t &rookAttacks, uint64_t &knightAttacks, uint64_t &bishopAttacks, uint64_t &queenAttacks, uint64_t &kingAttacks);
    bool isLegal(uint64_t& attackers);
    //bool isCheck(uint64_t& attackers);
    int movesFromIndex(int i, uint16_t* moves);
    void resetAttackers();
    void setAttackers(uint64_t &pawnAttacks, uint64_t &rookAttacks, uint64_t &knightAttacks, uint64_t &bishopAttacks, uint64_t &queenAttacks, uint64_t &kingAttacks);
    uint64_t getAttackers();

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