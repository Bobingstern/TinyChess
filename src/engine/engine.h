#pragma once

#include "../board/board.h"
#include <ctime>
#include <stdint.h>
#include <stdlib.h>
#include <vector>

#define USE_PST

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
    int staticEvaluation(uint64_t& attackers);
    
    int alphaBeta(int alpha, int beta, uint64_t attackers, int depthleft, int originalDepth, uint16_t &bestMove, bool &left, int &totalNodes, bool wasNull);
    int quiesce(int alpha, int beta, uint64_t attackers, int &totalNodes);
    bool badCapture(uint16_t move);
    uint16_t runSearch(int depth, int maxTime);
    uint16_t runSearchID(int maxTime);

    int pieceSquareTables();
    int pstScores(uint64_t a, int& scoreA, int& scoreB, int i);
    int passedPawn(uint64_t bb, uint64_t bb2);
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

    std::clock_t startClock;
    int maxTime = -1;
    int phaseEG = 0;
    int phaseMG = 0;

    int partition(uint16_t arr[], int start, int end);
    void quickSort(uint16_t arr[], int start, int end);
    bool mvvlvaOrder(uint16_t m);

  private:
    const uint64_t m1 = 0x5555555555555555;  // binary: 0101...
    const uint64_t m2 = 0x3333333333333333;  // binary: 00110011..
    const uint64_t m4 = 0x0f0f0f0f0f0f0f0f;  // binary:  4 zeros,  4 ones ...
    const uint64_t m8 = 0x00ff00ff00ff00ff;  // binary:  8 zeros,  8 ones ...
    const uint64_t m16 = 0x0000ffff0000ffff; // binary: 16 zeros, 16 ones ...
    const uint64_t m32 = 0x00000000ffffffff; // binary: 32 zeros, 32 ones
    const uint64_t h01 = 0x0101010101010101;

    int mg_value[6] = {85, 389, 409, 527, 1137, 0};
    int eg_value[6] = {103, 250, 281, 476, 880, 0};
    int gamephaseInc[12] = {0, 1, 1, 2, 4, 0};

    int16_t matValsMg[6] = {0, 1137, 527, 409, 389, 85};
    int16_t matValsEg[6] = {0, 880, 476, 281, 250, 103};

    uint64_t perft(int depth, uint64_t attackers, int originalDepth);
    int hammingWeight(uint64_t a);
    int getDistanceBB(uint64_t bb, int sq, int scale);
};
