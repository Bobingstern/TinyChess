#pragma once

#include "../board/board.h"
#include <stdint.h>
#include <stdlib.h>
#include <vector>

#define PAWN_MATERIAL_VALUE 100
#define KNIGHT_MATERIAL_VALUE 325
#define BISHOP_MATERIAL_VALUE 350
#define ROOK_MATERIAL_VALUE 500
#define QUEEN_MATERIAL_VALUE 900

class Engine {
  public:
    Engine();
    explicit Engine(Board* board);

    uint64_t runPerft(int depth);

    Board* board = nullptr;
    float staticEvaluation();
    
    float alphaBeta(float alpha, float beta, uint64_t attackers, int depthleft, int originalDepth, uint16_t &bestMove);
    float quiesce(float alpha, float beta, uint64_t attackers, int depthleft);
    uint16_t runSearch(int depth);

    float pieceSquareTables(int phase);
    int16_t pawnPST(int loc);
    int16_t egPawnPST(int loc);

    int16_t rookPST(int loc);
    int16_t egRookPST(int loc);

    int16_t knightPST(int loc);
    int16_t egKnightPST(int loc);

    int16_t bishopPST(int loc);
    int16_t egBishopPST(int loc);

    int16_t queenPST(int loc);
    int16_t egQueenPST(int loc);

    int16_t kingPST(int loc);
    int16_t egKingPST(int loc);

  private:
    const uint64_t m1 = 0x5555555555555555;  // binary: 0101...
    const uint64_t m2 = 0x3333333333333333;  // binary: 00110011..
    const uint64_t m4 = 0x0f0f0f0f0f0f0f0f;  // binary:  4 zeros,  4 ones ...
    const uint64_t m8 = 0x00ff00ff00ff00ff;  // binary:  8 zeros,  8 ones ...
    const uint64_t m16 = 0x0000ffff0000ffff; // binary: 16 zeros, 16 ones ...
    const uint64_t m32 = 0x00000000ffffffff; // binary: 32 zeros, 32 ones
    const uint64_t h01 = 0x0101010101010101;

    int pawnPhase = 0;
    int knightPhase = 1;
    int bishopPhase = 2;
    int rookPhase = 3;
    int queenPhase = 4;

    uint64_t perft(int depth, uint64_t attackers, int originalDepth);
    int hammingWeight(uint64_t a);
    
};
