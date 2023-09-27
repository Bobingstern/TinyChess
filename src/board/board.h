#pragma once

#include "squares.h"
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string>


#define bitRead(value, bit) (((value) >> (63 - bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (63 - bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (63 - bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, 63 - bit) : bitClear(value, 63 - bit))

#define WHITE 0
#define BLACK 1

#define BB_WHITE_PAWNS 0
#define BB_WHITE_ROOKS 1
#define BB_WHITE_KNIGHTS 2
#define BB_WHITE_BISHOPS 3
#define BB_WHITE_QUEENS 4
#define BB_WHITE_KING 5
#define BB_BLACK_PAWNS 6
#define BB_BLACK_ROOKS 7
#define BB_BLACK_KNIGHTS 8
#define BB_BLACK_BISHOPS 9
#define BB_BLACK_QUEENS 10
#define BB_BLACK_KING 11

// 00 00 000000 000000
// 2 bits unused
// 2 bits r for promo
// 6 r to index
// 6 r from index
#define moveReadFromIndex(move) (move & 0b0000000000111111)
#define moveReadToIndex(move) (move & 0b0000111111000000)
#define moveReadFlags(move) (move & 0b1111000000000000)
void moveSet(uint16_t& move, uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture,
             uint8_t castle);
uint16_t movePack(uint8_t from, uint8_t to, bool doublePawnPush, bool enPassant, uint8_t promo, bool capture,
                  uint8_t castle);

// https://www.chessprogramming.org/Encoding_Moves#From-To_Based
#define FLAG_QUIET_MOVE 0b0000000000000000
#define FLAG_DOUBLE_PAWN_PUSH 0b0001000000000000
#define FLAG_KING_CASTLE 0b0010000000000000
#define FLAG_QUEEN_CASTLE 0b0011000000000000
#define FLAG_CAPTURE 0b0100000000000000
#define FLAG_EN_PASSANT_CAPTURE 0b0101000000000000

#define FLAG_KNIGHT_PROMOTION 0b1000000000000000
#define FLAG_BISHOP_PROMOTION 0b1001000000000000
#define FLAG_ROOK_PROMOTION 0b1010000000000000
#define FLAG_QUEEN_PROMOTION 0b1011000000000000

#define FLAG_KNIGHT_PROMOTION_CAPTURE 0b1100000000000000
#define FLAG_BISHOP_PROMOTION_CAPTURE 0b1101000000000000
#define FLAG_ROOK_PROMOTION_CAPTURE 0b1110000000000000
#define FLAG_QUEEN_PROMOTION_CAPTURE 0b1111000000000000

class Board {
  public:
    Board();
    virtual ~Board();

    void printBoard();

    // Move generator
    void makeMove(uint16_t a);
    void unmakeMove(uint16_t a);

    bool color; // 0 == White

    int generateMoves(uint16_t* moves, uint64_t& pawnAttacks, uint64_t& rookAttacks, uint64_t& knightAttacks,
                      uint64_t& bishopAttacks, uint64_t& queenAttacks, uint64_t& kingAttacks);
    bool isLegal(uint64_t& attackers, bool wasInCheck);
    int movesFromIndex(int i, uint16_t* moves);
    void resetAttackers();
    void setAttackers(uint64_t& pawnAttacks, uint64_t& rookAttacks, uint64_t& knightAttacks, uint64_t& bishopAttacks,
                      uint64_t& queenAttacks, uint64_t& kingAttacks);
    uint64_t getAttackers();
    uint64_t getKing();
    int hammingWeight(uint64_t a);
    int pawnsCount(bool color);
    int knightsCount(bool color);
    int bishopsCount(bool color);
    int rooksCount(bool color);
    int queensCount(bool color);

    void printMove(uint16_t a, bool bin = false);

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

    void printBitBoard(uint64_t bb);

    void printAllBitBoards();

    void readFEN(std::string fen);
    int captures[128];
    int currentCapture = 0;
    int depth = 0;
    uint16_t previousMoves[128];
    uint16_t previousMover[128];
    int caps = 0;

    uint64_t kingAttacks(uint64_t isolated, uint8_t from);
    //position startpos moves d2d4 g8f6 c2c4 e7e6 g1f3 f8b4 c1d2 b8c6 d2b4 c6b4 h2h3 b4c6 f3e5 e8g8 g2g3 d8e8 f2f3 d7d6 e5g4 c8d7 g4h2 c6a5 e2e3 a8d8 b2b3 a5c6 a2a3 d6d5 c4c5 a7a6 h3h4 f6h5 h1g1 h5f6 g3g4 h7h6 f3f4 g7g5 h4g5 h6g5 f4g5 f6e4 h2f3 e6e5 f3e5 c6e5 d4e5 e8e5 a1a2 e5g5 d1c1 d7c6 b3b4 f7f5 f1d3 g5h4 e1d1 e4f2 a2f2 h4f2 g1h1 f5g4 d3h7 g8f7 b1d2 g4g3 h7c2 g3g2 c2g6 f7g6 c1b1 g6g5 h1h5 g5h5 b1h7 h5g4 h7g6 g4h3 g6h6 f2h4 h6e6 h4g4 e6g4 h3g4 e3e4 g2g1q d2f1 f8f2 d1c1 d5e4 c1b1 e4e3 b1a1 a6a5 a1b1 a5b4 b1a1 b4a3 a1b1 a3a2 b1a1 b7b5
  private:
    void sliceReAdd();
    // History
    bool fenEnpassant = false;
    // Utilities
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
    

    uint64_t pawnAttackers;
    uint64_t rookAttackers;
    uint64_t knightAttackers;
    uint64_t bishopAttackers;
    uint64_t queenAttackers;
    uint64_t kingAttackers;

    // Castling flags
    bool flagWhiteKingsideCastle = 1; // 1 means it can castle
    int wkscDepth = -1;               // White king sidecastle trip depth

    bool flagWhiteQueensideCastle = 1;
    int wqscDepth = -1;

    bool flagBlackKingsideCastle = 1;
    int bkscDepth = -1;

    bool flagBlackQueensideCastle = 1;
    int bqscDepth = -1;

    // Magic not bb
    const uint64_t m1 = 0x5555555555555555;  // binary: 0101...
    const uint64_t m2 = 0x3333333333333333;  // binary: 00110011..
    const uint64_t m4 = 0x0f0f0f0f0f0f0f0f;  // binary:  4 zeros,  4 ones ...
    const uint64_t m8 = 0x00ff00ff00ff00ff;  // binary:  8 zeros,  8 ones ...
    const uint64_t m16 = 0x0000ffff0000ffff; // binary: 16 zeros, 16 ones ...
    const uint64_t m32 = 0x00000000ffffffff; // binary: 32 zeros, 32 ones
    const uint64_t h01 = 0x0101010101010101;

    uint16_t pawnMoves(bool color, uint16_t* moves, int i);
    uint16_t rookMoves(bool color, uint16_t* moves, int i);
    uint16_t bishopMoves(bool color, uint16_t* moves, int i);
    uint16_t queenMoves(bool color, uint16_t* moves, int i);
    uint16_t knightMoves(bool color, uint16_t* moves, int i);
    uint16_t kingMoves(bool color, uint16_t* moves, int i);

    // bool isCheck(uint64_t& attackers);

    uint64_t** bitboards = nullptr;
};