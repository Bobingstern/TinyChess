#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int Engine::hammingWeight(uint64_t x) {
  x -= (x >> 1) & m1;             // put count of each 2 bits into those 2 bits
  x = (x & m2) + ((x >> 2) & m2); // put count of each 4 bits into those 4 bits
  x = (x + (x >> 4)) & m4;        // put count of each 8 bits into those 8 bits
  return (x * h01) >> 56;         // returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24)
}

// float Engine::pawnPST(int loc){
//     // Temporary
//     int16_t mg_pawn_table[64] = {
//       0,   0,   0,   0,   0,   0,  0,   0,
//      98, 134,  61,  95,  68, 126, 34, -11,
//      -6,   7,  26,  31,  65,  56, 25, -20,
//     -14,  13,   6,  21,  23,  12, 17, -23,
//     -27,  -2,  -5,  12,  17,   6, 10, -25,
//     -26,  -4,  -4, -10,   3,   3, 33, -12,
//     -35,  -1, -20, -23, -15,  24, 38, -22,
//       0,   0,   0,   0,   0,   0,  0,   0,
//     };
//     return mg_pawn_table[ this->board->color == WHITE ? loc : 63 - loc];
// }
// float Engine::knightPST(int loc){
//     // Temporary
//     int16_t mg_knight_table[64] = {
//     -167, -89, -34, -49,  61, -97, -15, -107,
//      -73, -41,  72,  36,  23,  62,   7,  -17,
//      -47,  60,  37,  65,  84, 129,  73,   44,
//       -9,  17,  19,  53,  37,  69,  18,   22,
//      -13,   4,  16,  13,  28,  19,  21,   -8,
//      -23,  -9,  12,  10,  19,  17,  25,  -16,
//      -29, -53, -12,  -3,  -1,  18, -14,  -19,
//     -105, -21, -58, -33, -17, -28, -19,  -23,
//     };
//     return mg_knight_table[ this->board->color == WHITE ? loc : 63 - loc];
// }

// float Engine::bishopPST(int loc){
//     // Temporary
//     int16_t mg_bishop_table[64] = {
//     -29,   4, -82, -37, -25, -42,   7,  -8,
//     -26,  16, -18, -13,  30,  59,  18, -47,
//     -16,  37,  43,  40,  35,  50,  37,  -2,
//      -4,   5,  19,  50,  37,  37,   7,  -2,
//      -6,  13,  13,  26,  34,  12,  10,   4,
//       0,  15,  15,  15,  14,  27,  18,  10,
//       4,  15,  16,   0,   7,  21,  33,   1,
//     -33,  -3, -14, -21, -13, -12, -39, -21,
//     };
//     return mg_bishop_table[ this->board->color == WHITE ? loc : 63 - loc];
// }

// float Engine::rookPST(int loc){
//     // Temporary
//     int16_t mg_rook_table[64] = {
//     32,  42,  32,  51, 63,  9,  31,  43,
//      27,  32,  58,  62, 80, 67,  26,  44,
//      -5,  19,  26,  36, 17, 45,  61,  16,
//     -24, -11,   7,  26, 24, 35,  -8, -20,
//     -36, -26, -12,  -1,  9, -7,   6, -23,
//     -45, -25, -16, -17,  3,  0,  -5, -33,
//     -44, -16, -20,  -9, -1, 11,  -6, -71,
//     -19, -13,   1,  17, 16,  7, -37, -26,
//     };
//     return mg_rook_table[ this->board->color == WHITE ? loc : 63 - loc];
// }
// float Engine::queenPST(int loc){
//     // Temporary
//     int16_t mg_queen_table[64] = {
//     -28,   0,  29,  12,  59,  44,  43,  45,
//     -24, -39,  -5,   1, -16,  57,  28,  54,
//     -13, -17,   7,   8,  29,  56,  47,  57,
//     -27, -27, -16, -16,  -1,  17,  -2,   1,
//      -9, -26,  -9, -10,  -2,  -4,   3,  -3,
//     -14,   2, -11,  -2,  -5,   2,  14,   5,
//     -35,  -8,  11,   2,   8,  15,  -3,   1,
//      -1, -18,  -9,  10, -15, -25, -31, -50,
//     };
//     return mg_queen_table[ this->board->color == WHITE ? loc : 63 - loc];
// }

float Engine::pieceSquareTables(int phase){
    // Pawns
    float score = 0;
    uint64_t cpy = this->board->color == 0 ? this->board->whitePawns : this->board->blackPawns;
    while (cpy != 0) {
        uint64_t isolated = cpy & ((~cpy) + 1);
        int from = 63 - __builtin_ctzll(isolated);
        score += (( pawnPST(from) * (256 - phase)) + (egPawnPST(from) * phase)) / 256;
        cpy &= ~isolated;
    }
    cpy = this->board->color == 0 ? this->board->whiteKnights : this->board->blackKnights;
    while (cpy != 0) {
        uint64_t isolated = cpy & ((~cpy) + 1);
        int from = 63 - __builtin_ctzll(isolated);
        score += (( knightPST(from) * (256 - phase)) + (egKnightPST(from) * phase)) / 256;
        cpy &= ~isolated;
    }
    cpy = this->board->color == 0 ? this->board->whiteBishops : this->board->blackBishops;
    while (cpy != 0) {
        uint64_t isolated = cpy & ((~cpy) + 1);
        int from = 63 - __builtin_ctzll(isolated);
        score += (( bishopPST(from) * (256 - phase)) + (egBishopPST(from) * phase)) / 256;
        
        cpy &= ~isolated;
    }
    cpy = this->board->color == 0 ? this->board->whiteRooks : this->board->blackRooks;
    while (cpy != 0) {
        uint64_t isolated = cpy & ((~cpy) + 1);
        int from = 63 - __builtin_ctzll(isolated);
        score += (( rookPST(from) * (256 - phase)) + (egRookPST(from) * phase)) / 256;
        cpy &= ~isolated;
    }
    cpy = this->board->color == 0 ? this->board->whiteQueens : this->board->blackQueens;
    while (cpy != 0) {
        uint64_t isolated = cpy & ((~cpy) + 1);
        int from = 63 - __builtin_ctzll(isolated);
        score += (( queenPST(from) * (256 - phase)) + (egQueenPST(from) * phase)) / 256;
        cpy &= ~isolated;
    }
    // cpy = this->board->color == 0 ? this->board->whiteKing : this->board->blackKing;
    // while (cpy != 0) {
    //     uint64_t isolated = cpy & ((~cpy) + 1);
    //     int from = 63 - __builtin_ctzll(isolated);
    //     score += (( kingPST(from) * (256 - phase)) + (egKingPST(from) * phase)) / 256;
    //     cpy &= ~isolated;
    // }
    return score;
}

float Engine::staticEvaluation(){
    // Sided
    int whitePawns = this->board->pawnsCount(0);
    int blackPawns = this->board->pawnsCount(1);

    int whiteKnights = this->board->knightsCount(0);
    int blackKnights = this->board->knightsCount(1);

    int whiteBishops = this->board->bishopsCount(0);
    int blackBishops = this->board->bishopsCount(1);

    int whiteRooks = this->board->rooksCount(0);
    int blackRooks = this->board->rooksCount(1);

    int whiteQueens = this->board->queensCount(0);
    int blackQueens = this->board->queensCount(1);

    float whiteMaterial = whitePawns * PAWN_MATERIAL_VALUE
                        + whiteKnights * KNIGHT_MATERIAL_VALUE
                        + whiteBishops * BISHOP_MATERIAL_VALUE
                        + whiteRooks * ROOK_MATERIAL_VALUE
                        + whiteQueens * QUEEN_MATERIAL_VALUE;

    float blackMaterial = blackPawns * PAWN_MATERIAL_VALUE
                        + blackKnights * KNIGHT_MATERIAL_VALUE
                        + blackBishops * BISHOP_MATERIAL_VALUE
                        + blackRooks * ROOK_MATERIAL_VALUE
                        + blackQueens * QUEEN_MATERIAL_VALUE;
    float MD = whiteMaterial - blackMaterial;
    MD = MD < 0 ? -MD : MD;
    float PA = whiteMaterial > blackMaterial ? whitePawns : blackPawns;
    float MS = std::min(2400.0f, MD) + (MD * PA * (8000 - (whiteMaterial + blackMaterial))) / (6400 * (PA + 1));
    MS = std::min(3100.0f, MS);
    //return MS * (this->board->color == 1 ? -1 : 1);
    // White has more material
    if (whiteMaterial > blackMaterial){
        if (this->board->color == 1){
            MS *= -1;
        }
    }
    else {
        if (this->board->color == 0){
            MS *= -1;
        }
    }


    //Calculate phase bs
    int totalPhase = pawnPhase*16 + knightPhase*4 + bishopPhase*4 + rookPhase*4 + queenPhase*2;
    int phase = totalPhase;
    phase -= (whitePawns + blackPawns) * pawnPhase;
    phase -= (whiteKnights + blackKnights) * knightPhase;
    phase -= (whiteBishops + blackBishops) * bishopPhase;
    phase -= (whiteRooks + blackRooks) * rookPhase;
    phase -= (whiteQueens + blackQueens) * queenPhase;
    phase = (phase * 256 + (totalPhase / 2)) / totalPhase;

    return MS + pieceSquareTables(phase);
}