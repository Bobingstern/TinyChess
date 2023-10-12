#pragma once

#include "../board/board.h"
#include <ctime>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>

#define USE_PST

#define PAWN_MATERIAL_VALUE 100
#define KNIGHT_MATERIAL_VALUE 325
#define BISHOP_MATERIAL_VALUE 350
#define ROOK_MATERIAL_VALUE 500
#define QUEEN_MATERIAL_VALUE 900

struct S{
  int8_t mg;
  int8_t eg;

  S(int8_t a, int8_t b){
    mg = a;
    eg = b;
  };
};

struct S32{
  int mg;
  int eg;

  S32(int a, int b){
    mg = a;
    eg = b;
  };
};

class Engine {
  public:
    Engine();
    explicit Engine(Board* board);

    uint64_t runPerft(int depth);

    Board* board = nullptr;
    int staticEvaluation(uint64_t& attackers);
    
    int alphaBeta(int alpha, int beta, uint64_t attackers, int depthleft, int originalDepth, uint16_t &bestMove, bool &left, int &totalNodes, bool wasNull);
    int pvSearch(int alpha, int beta, uint64_t attackers, int depthleft, int originalDepth, uint16_t &bestMove, bool &left, int &totalNodes, bool wasNull);
    int zwSearch(int beta, uint64_t attackers, int depthleft, int originalDepth, uint16_t &bestMove, bool &left, int &totalNodes, bool wasNull);
    int quiesce(int alpha, int beta, uint64_t attackers, int &totalNodes);
    bool badCapture(uint16_t move);
    uint16_t runSearch(int depth, int maxTime);
    uint16_t runSearchID(int maxTime, int& score, int& nodes);
    int getEval();
    int32_t packScore(int16_t mg, int16_t eg);
    int16_t unpack_mg(int32_t packed);
    int16_t unpack_eg(int32_t packed);

    int pieceSquareTables(int &mgScore, int &egScore);
    int pstScores(uint64_t a, int& scoreA, int& scoreB, int i);
    int passedPawn(uint64_t bb, uint64_t bb2, int& mgScore, int& egScore);

    int8_t pawnPST(int loc);
    int8_t egPawnPST(int loc);

    int8_t rookPST(int loc);
    int8_t egRookPST(int loc);

    int8_t knightPST(int loc);
    int8_t egKnightPST(int loc);

    int8_t bishopPST(int loc);
    int8_t egBishopPST(int loc);

    int8_t queenPST(int loc);
    int8_t egQueenPST(int loc);

    int8_t kingPST(int loc);
    int8_t egKingPST(int loc);

    std::clock_t startClock;
    int maxTime = -1;
    int phaseEG = 0;
    int phaseMG = 0;

    int partition(uint16_t arr[], int start, int end);
    void quickSort(uint16_t arr[], int start, int end);
    int mvvlvaOrder(uint16_t m);

    // int evalScoreTuner(std::string L);
    // int8_t rookMobilityMG[15] = { -6, -3, -3, -3, -3, -2, -2, -1,  0,  2,  2,  2,  3,  2,  2 };
    // int8_t rookMobilityEG[15] = { -6, -5, -2,  1,  4,  4,  6,  7,  8,  9, 10, 10, 10, 10, 10 };

    // int8_t bishopMobilityMG[14] = { -6, -3, -3, -3, -3, -2, -2, -1,  0,  2,  2,  2,  3,  2 };
    // int8_t bishopMobilityEG[14] = { -6, -5, -2,  1,  4,  4,  6,  7,  8,  9, 10, 10, 10, 10 };
    S rookMobility[15] = {S(1, 1), S(1, 1), S(-44, -19), S(-35, -6), S(-27, 2), S(-23, 6), S(-20, 10), S(-16, 17), S(-9, 18), S(-2, 21), S(4, 26), S(10, 28), S(13, 32), S(20, 35), S(23, 35)};
    S bishopMobility[14] = {S(1, 1), S(-54, -65), S(-41, -55), S(-34, -22), S(-21, -12), S(-15, -3), S(-2, 12), S(5, 18), S(12, 30), S(14, 33), S(19, 40), S(22, 35), S(22, 35), S(51, 25)};
    S queenMobility[28] = {S(1, 1), S(1, 1), S(1, 1), S(-54, -15), S(-58, -122), S(-28, -77), S(-26, -54), S(-27, 7), S(-22, 21), S(-21, 42), S(-17, 47), S(-14, 59), S(-11, 70), S(-9, 73), S(-5, 79), S(-4, 87), S(-3, 95), S(-1, 102), S(-1, 109), S(0, 113), S(1, 124), S(5, 123), S(5, 127), S(14, 124), S(15, 123), S(28, 123), S(72, 99), S(127, 80)};
  private:
    const uint64_t m1 = 0x5555555555555555;  // binary: 0101...
    const uint64_t m2 = 0x3333333333333333;  // binary: 00110011..
    const uint64_t m4 = 0x0f0f0f0f0f0f0f0f;  // binary:  4 zeros,  4 ones ...
    const uint64_t m8 = 0x00ff00ff00ff00ff;  // binary:  8 zeros,  8 ones ...
    const uint64_t m16 = 0x0000ffff0000ffff; // binary: 16 zeros, 16 ones ...
    const uint64_t m32 = 0x00000000ffffffff; // binary: 32 zeros, 32 ones
    const uint64_t h01 = 0x0101010101010101;


    S32 material[6] = {S32(78, 193), S32(273, 363), S32(318, 363), S32(415, 640), S32(868, 1142), S32(0, 0)};
    int gamephaseInc[12] = {0, 1, 1, 2, 4, 0};
    
    // int mg_value[6] = {85, 389, 409, 527, 1137, 0};
    // int eg_value[6] = {103, 250, 281, 476, 880, 0};

    
    uint64_t perft(int depth, uint64_t attackers, int originalDepth);
    int hammingWeight(uint64_t a);
    int getDistanceBB(uint64_t bb, int sq, int scale);
    int doubledPawn(int& mg, int& eg, bool color);
};
