#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// const int8_t pst_rank[][8] = {
//     {0, S(-3, 0), S(-3, -1), S(-1, -1), S(2, 0), S(5, 2), 0, 0},
//     {S(-6, -5), S(-4, -2), S(-1, 0), S(1, 3), S(5, 4), S(10, 1), S(6, -1), S(-11, -1)},
//     {S(-5, -3), S(-1, -2), S(1, 0), S(2, 2), S(3, 2), S(6, 1), S(2, 0), S(-8, 0)},
//     {S(-2, -2), S(-4, -3), S(-5, -2), S(-4, 0), S(-1, 1), S(3, 1), S(5, 2), S(8, 1)},
//     {S(-2, -11), S(0, -9), S(0, -4), S(-2, 2), S(-1, 6), S(3, 4), S(-1, 7), S(4, 4)},
//     {S(0, -5), S(0, -1), S(-2, 0), S(-5, 2), S(-2, 3), S(6, 2), S(3, 1), S(2, -4)},
// };
// const int8_t pst_file[][8] = {
//     {S(-2, 0), S(-1, 1), S(-1, 0), S(0, -1), S(1, 0), S(2, 0), S(3, 0), S(-2, 0)},
//     {S(-6, -3), S(-2, -1), S(1, 2), S(2, 3), S(2, 3), S(3, 1), S(1, -1), S(-2, -4)},
//     {S(-3, -2), 0, S(1, 0), S(0, 2), S(0, 2), S(0, 1), S(2, -1), S(-1, -2)},
//     {S(-1, 0), S(-2, 1), S(-1, 1), 0, S(1, -1), S(2, 0), S(2, 0), S(-1, -1)},
//     {S(-3, -5), S(-2, -2), S(-1, 0), S(0, 1), S(0, 3), S(1, 3), S(3, 0), S(2, 0)},
//     {S(-2, -3), S(1, -1), S(-3, 1), S(-5, 2), S(-5, 2), S(-2, 1), S(1, 0), S(1, -3)},
// };

// {
//     {0, 1137, 527, 409, 389, 85, }
//     {0, 880, 476, 281, 250, 103, }
//     {
//         {  -8,   -4,  -27,  -83,  -31,  -57,    8,    0, },
//         { -17,   -7,   -4,    2,   -7,    2,    3,   11, },
//         { -14,  -12,    5,   10,   14,   -1,  -14,   -4, },
//         { -11,   11,   15,    7,    8,    5,   19,    0, },
//         { -31,   -7,    7,   10,    7,   11,   -3,  -21, },
//         { -45,  -13,  -14,   -2,    7,    9,   36,  -14, },
//     },
//     {
//         { -37,  -12,    2,   15,    6,   16,   -3,  -21, },
//         { -27,  -22,   -4,   -5,   21,    8,   -7,  -29, },
//         {  -1,    5,    3,    5,   -4,   -3,   -4,  -19, },
//         { -20,  -14,  -13,   -4,   -2,   -7,  -19,  -28, },
//         { -31,  -13,   -5,    6,    4,   -5,  -16,  -33, },
//         {  24,   14,    4,   -2,   -8,   -5,   -2,    1, },
//     },

//     {
//         {  26,   -1,  -29,  -22,   25,   85,   89,  118, },
//         {  -1,    4,    0,   -8,  -11,   28,  -13,   -7, },
//         {   1,  -19,   -9,  -12,   14,   33,   39,   77, },
//         { -12,    8,   16,   13,    3,   36,   11,  -32, },
//         { -22,   -9,    2,    5,   15,   45,   22, -127, },
//         {   0,    5,    0,   -1,    5,   13,   30,    0, },
//     },
//     {
//         { -41,   -4,    5,    4,    0,   -5,  -13,  -42, },
//         { -59,  -29,  -16,    7,   20,   -6,   20,    8, },
//         {  -8,   -6,  -12,   -4,   -5,   -6,   -1,  -10, },
//         { -29,  -12,   -9,   -6,    0,  -11,  -22,  -22, },
//         { -32,  -15,   -2,   10,   13,   -9,  -27,  -22, },
//         {   0,   -7,  -14,  -17,    0,   65,  141,    0, },
//     },
// }
int16_t Engine::pawnPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;

  const int8_t fileBonus[] = { -45,  -13,  -14,   -2,    7,    9,   36,  -14};
  const int8_t rankBonus[] = {   0,    5,    0,   -1,    5,   13,   30,    0};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::egPawnPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = {  24,   14,    4,   -2,   -8,   -5,   -2,    1};
  const int8_t rankBonus[] = {   0,   -7,  -14,  -17,    0,   65,  127,    0};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::knightPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -31,   -7,    7,   10,    7,   11,   -3,  -21};
  const int8_t rankBonus[] = { -22,   -9,    2,    5,   15,   45,   22, -127};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::egKnightPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -31,  -13,   -5,    6,    4,   -5,  -16,  -33};
  const int8_t rankBonus[] = { -32,  -15,   -2,   10,   13,   -9,  -27,  -22};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::bishopPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -11,   11,   15,    7,    8,    5,   19,    0, };
  const int8_t rankBonus[] = { -12,    8,   16,   13,    3,   36,   11,  -32, };
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::egBishopPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -20,  -14,  -13,   -4,   -2,   -7,  -19,  -28};
  const int8_t rankBonus[] = { -29,  -12,   -9,   -6,    0,  -11,  -22,  -22};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::rookPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -14,  -12,    5,   10,   14,   -1,  -14,   -4};
  const int8_t rankBonus[] = {   1,  -19,   -9,  -12,   14,   33,   39,   77};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];

}

int16_t Engine::egRookPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = {  -1,    5,    3,    5,   -4,   -3,   -4,  -19};
  const int8_t rankBonus[] = {  -8,   -6,  -12,   -4,   -5,   -6,   -1,  -10};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::queenPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -17,   -7,   -4,    2,   -7,    2,    3,   11};
  const int8_t rankBonus[] = {  -1,    4,    0,   -8,  -11,   28,  -13,   -7};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::egQueenPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -27,  -22,   -4,   -5,   21,    8,   -7,  -29};
  const int8_t rankBonus[] = { -59,  -29,  -16,    7,   20,   -6,   20,    8};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::kingPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = {  -8,   -4,  -27,  -83,  -31,  -57,    8,    0};
  const int8_t rankBonus[] = {  26,   -1,  -29,  -22,   25,   85,   89,  118};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}

int16_t Engine::egKingPST(int loc) {
  const int square_index = this->board->color == WHITE ? loc : loc ^ 56;
  const int8_t fileBonus[] = { -37,  -12,    2,   15,    6,   16,   -3,  -21};
  const int8_t rankBonus[] = { -41,   -4,    5,    4,    0,   -5,  -13,  -42};
  return fileBonus[square_index % 8] + rankBonus[(int)(square_index / 8 )];
}
