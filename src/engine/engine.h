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
    int pvSearch(int alpha, int beta, uint64_t attackers, int depthleft, int originalDepth, uint16_t &bestMove, bool &left, int &totalNodes, bool wasNull, uint16_t prevBest);
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
    S rookMobility[15] = {S(1, 1), S(1, 1), S(-40, -10), S(-31, 3), S(-24, 11), S(-20, 15), S(-17, 18), S(-14, 23), S(-9, 24), S(-3, 27), S(1, 32), S(5, 34), S(6, 38), S(11, 42), S(12, 43)};
    S bishopMobility[14] = {S(1, 1), S(-52, -63), S(-40, -56), S(-32, -23), S(-20, -13), S(-15, -3), S(-3, 13), S(3, 18), S(8, 31), S(9, 35), S(11, 42), S(11, 38), S(9, 39), S(33, 30)};
    
    S queenMobility[28] = {S(1, 1), S(1, 1), S(1, 1), S(-62, -32), S(-53, -122), S(-22, -77), S(-21, -50), S(-22, 9), S(-17, 22), S(-17, 40), S(-13, 44), S(-10, 56), S(-8, 67), S(-6, 71), S(-3, 76), S(-3, 85), S(-3, 92), S(-2, 99), S(-2, 105), S(-1, 110), S(-1, 119), S(1, 118), S(2, 120), S(10, 116), S(12, 114), S(23, 112), S(60, 94), S(110, 75)};
    
    S kingDefenders[3] =  {S(-20, 5), S(-24, 6), S(-17, -4)};

  private:
    S32 material[6] = {S32(78, 193), S32(273, 363), S32(318, 363), S32(415, 640), S32(868, 1142), S32(0, 0)};
    int gamephaseInc[12] = {0, 1, 1, 2, 4, 0};
    
    // int mg_value[6] = {85, 389, 409, 527, 1137, 0};
    // int eg_value[6] = {103, 250, 281, 476, 880, 0};

    
    uint64_t perft(int depth, uint64_t attackers, int originalDepth);
    int hammingWeight(uint64_t a);
    int getDistanceBB(uint64_t bb, int sq, int scale);
    int doubledPawn(int& mg, int& eg, bool color);
};
