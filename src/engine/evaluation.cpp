#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

/* board representation */
#define WHITE 0
#define BLACK 1
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

int32_t Engine::packScore(int16_t mg, int16_t eg){
  return ((int32_t) eg << 16) + (int32_t) mg;
}
int16_t Engine::unpack_mg(int32_t packed) {
    return (int16_t) packed;
}

int16_t Engine::unpack_eg(int32_t packed) {
    return (int16_t) ((packed + 0x8000) >> 16);
}

int Engine::pstScores(uint64_t a, int& mg, int& eg, int i) {
  int gamePhase = 0;
  uint64_t cpy = a;
  while (cpy != 0) {
    uint64_t isolated = cpy & ((~cpy) + 1);
    int from = 63 - __builtin_ctzll(isolated);
    if (i == 0) {
      mg += pawnPST(from) + material[i].mg;
      eg += egPawnPST(from) + material[i].eg;
      
    }
    if (i == 1) {
      mg += knightPST(from) + material[i].mg;
      eg += egKnightPST(from) + material[i].eg;
    }
    if (i == 2) {
      mg += bishopPST(from) + material[i].mg;
      eg += egBishopPST(from) + material[i].eg;
    }
    if (i == 3) {
      mg += rookPST(from) + material[i].mg;
      eg += egRookPST(from) + material[i].eg;
    }
    if (i == 4) {
      mg += queenPST(from) + material[i].mg;
      eg += egQueenPST(from) + material[i].eg;
    }
    if (i == 5) {
      mg += kingPST(from) + material[i].mg;
      eg += egKingPST(from) + material[i].eg;
    }
    cpy &= ~isolated;
    gamePhase++;
  }
  return gamePhase;
}
int Engine::pieceSquareTables(int &mgS, int &egS) {
  // Pawns
  int score = 0;
  int whiteMG = 0;
  int whiteEG = 0;
  int blackMG = 0;
  int blackEG = 0;
  int gamePhase = 0;
  bool currColor = board->color;
  board->color = WHITE;
  gamePhase += (pstScores(board->whitePawns, whiteMG, whiteEG, 0)) * gamephaseInc[0];
  board->color = BLACK;
  gamePhase += (pstScores(board->blackPawns, blackMG, blackEG, 0)) * gamephaseInc[0];

  board->color = WHITE;
  gamePhase += (pstScores(board->whiteKnights, whiteMG, whiteEG, 1)) * gamephaseInc[1];
  board->color = BLACK;
  gamePhase += (pstScores(board->blackKnights, blackMG, blackEG, 1)) * gamephaseInc[1];

  board->color = WHITE;
  gamePhase += (pstScores(board->whiteBishops, whiteMG, whiteEG, 2)) * gamephaseInc[2];
  board->color = BLACK;
  gamePhase += (pstScores(board->blackBishops, blackMG, blackEG, 2)) * gamephaseInc[2];

  board->color = WHITE;
  gamePhase += (pstScores(board->whiteRooks, whiteMG, whiteEG, 3)) * gamephaseInc[3];
  board->color = BLACK;
  gamePhase += (pstScores(board->blackRooks, blackMG, blackEG, 3)) * gamephaseInc[3];

  board->color = WHITE;
  gamePhase += (pstScores(board->whiteQueens, whiteMG, whiteEG, 4)) * gamephaseInc[4];
  board->color = BLACK;
  gamePhase += (pstScores(board->blackQueens, blackMG, blackEG, 4)) * gamephaseInc[4];

  board->color = WHITE;
  gamePhase += (pstScores(board->whiteKing, whiteMG, whiteEG, 5)) * gamephaseInc[5];
  board->color = BLACK;
  gamePhase += (pstScores(board->blackKing, blackMG, blackEG, 5)) * gamephaseInc[5];

  board->color = currColor;

  
  int mgScore = board->color == WHITE ? whiteMG - blackMG : blackMG - whiteMG;
  int egScore = board->color == WHITE ? whiteEG - blackEG : blackEG - whiteEG;
  mgS += mgScore;
  egS += egScore;
  //std::cout << whiteMG << " " << blackMG << " " << mgScore << "\n";
  //std::cout << mgS << "\n";
 
  int mgPhase = gamePhase;
  if (mgPhase > 24)
    mgPhase = 24;
  int egPhase = 24 - mgPhase;
  phaseEG = egPhase;
  phaseMG = mgPhase;
  
  return (mgScore * mgPhase + egScore * egPhase) / 24;
}

int Engine::getDistanceBB(uint64_t bb, int sq, int scale){
  uint64_t cpy = bb;
  int result = 0;
  while (cpy != 0) {
    uint64_t isolated = cpy & ((~cpy) + 1);
    int from = 63 - __builtin_ctzll(isolated);
    result += board->distance(sq, from) * scale;
    cpy &= ~isolated;
  }
  return result;
}

int Engine::doubledPawn(int& mg, int& eg){
  uint64_t cpy = board->color == WHITE ? board->whitePawns : board->blackPawns;;
  int result = 0;
  while (cpy != 0) {
    uint64_t isolated = cpy & ((~cpy) + 1);
    if (board->color == WHITE){
      if (((isolated << 8) & board->whitePawns) != 0){
        mg -= 5;
        eg -= 10;
      }
      else if (((isolated << 16) & board->whitePawns) != 0){
        mg -= 5;
        eg -= 10;
      }
    }
    if (board->color == BLACK){
      if (((isolated >> 8) & board->blackPawns) != 0){
        mg -= 7;
        eg -= 22;
      }
      else if (((isolated >> 16) & board->blackPawns) != 0){
        mg -= 7;
        eg -= 22;
      }
    }
    cpy &= ~isolated;
  }
  return result;
}

int Engine::passedPawn(uint64_t bb, uint64_t bb2, int& mgScore, int& egScore){
  uint64_t cpy = bb;
  bool result = true;
  int cnt = 0;
  int8_t bonusMG[8] = {
    0, 2, 15, 22, 64, 127, 127, 0
  };
  int8_t bonusEG[8] = {
    0, 38, 36, 50, 81, 127, 127, 0
  };
  while (cpy != 0) {
    uint64_t isolated = cpy & ((~cpy) + 1);
    int from = 63 - __builtin_ctzll(isolated);
    if (((0x101010101010101ULL << (uint8_t)(7 - (from % 8))) & bb2 ) != 0){
      result = false;
    }
    // if (from % 8 != 0 && ((0x101010101010101ULL << (uint8_t)(7 - ((from-1) % 8))) & bb2 ) != 0){
    //   result = false;
    // }
    // if ((from+1) % 8 != 0 && ((0x101010101010101ULL << (uint8_t)(7 - ((from+1) % 8))) & bb2 ) != 0){
    //   result = false;
    // }
    if (result){
      int f = from % 8;
      mgScore += bonusMG[from];
      egScore += bonusEG[from];
    }
    
    result = true;
    cpy &= ~isolated;
  }
  return cnt;
}

int Engine::staticEvaluation(uint64_t& attackers) {
  // Sided
  
  // Get king distance attackers
#ifdef USE_PST
  int mgScore = 0;
  int egScore = 0;
  int psts = pieceSquareTables(mgScore, egScore);
  int score = 0;
  //std::cout << mgScore << " " << egScore << " " << phaseMG << " " << phaseEG << "\n";
  // King saftey
  int kingDanger = 0;
  //kingDanger -= 8 * ( board->color == WHITE ? board->blackQueens == 0 : board->whiteQueens == 0);
  //kingDanger += hammingWeight( board->kingAttacks(board->getKing(), 63 - __builtin_ctzll(board->getKing())) & attackers) * 3;
  int kingFrom = 63 - __builtin_ctzll(board->getKing());
  uint64_t kingArea = board->kingAttacks(board->getKing(), kingFrom);
  int knightScore = hammingWeight(kingArea & board->knightAttackers);
  int bishopScore = hammingWeight(kingArea & board->bishopAttackers);
  int rookScore = hammingWeight(kingArea & board->rookAttackers);
  int queenScore = hammingWeight(kingArea & board->queenAttackers);
  int valueOfAttacks = knightScore * 5 + bishopScore * 8 + rookScore * 15 + queenScore * 25;
  //kingDanger += hammingWeight(kingArea & (board->knightAttackers | board->bishopAttackers | board->rookAttackers | board->queenAttackers)) * (valueOfAttacks);

  // uint64_t kingFlank = (0x101010101010101ULL << (uint8_t)(7 - (kingFrom % 8)));
  // int flankDefense = 0;
  // if (kingFrom % 8 != 0){
  //   kingFlank |= (0x101010101010101ULL << (uint8_t)(7 - ((kingFrom-1) % 8)));
  // }
  // if ((kingFrom+1) % 8 != 0){
  //   kingFlank |= (0x101010101010101ULL << (uint8_t)(7 - ((kingFrom+1) % 8)));
  // }

  //Rook Mobility
  uint64_t cpy = board->color == WHITE ? board->whiteRooks : board->blackRooks;
  while (cpy != 0) {
    uint64_t isolatedRook = cpy & ((~cpy) + 1);
    uint8_t from = 63 - __builtin_ctzll(isolatedRook);
    uint64_t attacks = board->rookAttacks(from);
    mgScore += rookMobility[hammingWeight(attacks)].mg;
    egScore += rookMobility[hammingWeight(attacks)].eg;
    cpy &= ~isolatedRook;
  }
  cpy = board->color == BLACK ? board->whiteRooks : board->blackRooks;
  while (cpy != 0) {
    uint64_t isolatedRook = cpy & ((~cpy) + 1);
    uint8_t from = 63 - __builtin_ctzll(isolatedRook);
    uint64_t attacks = board->rookAttacks(from);
    mgScore -= rookMobility[hammingWeight(attacks)].mg;
    egScore -= rookMobility[hammingWeight(attacks)].eg;
    cpy &= ~isolatedRook;
  }
  // Bishop Mobility
  cpy = board->color == WHITE ? board->whiteBishops : board->blackBishops;
  while (cpy != 0) {
    uint64_t isolatedRook = cpy & ((~cpy) + 1);
    uint8_t from = 63 - __builtin_ctzll(isolatedRook);
    uint64_t attacks = board->bishopAttacks(from);
    mgScore += bishopMobility[hammingWeight(attacks)].mg;
    egScore += bishopMobility[hammingWeight(attacks)].eg;
    cpy &= ~isolatedRook;
  }
  cpy = board->color == BLACK ? board->whiteBishops : board->blackBishops;
  while (cpy != 0) {
    uint64_t isolatedRook = cpy & ((~cpy) + 1);
    uint8_t from = 63 - __builtin_ctzll(isolatedRook);
    uint64_t attacks = board->bishopAttacks(from);
    mgScore -= bishopMobility[hammingWeight(attacks)].mg;
    egScore -= bishopMobility[hammingWeight(attacks)].eg;
    cpy &= ~isolatedRook;
  }
  
  // if (hammingWeight(board->color == WHITE ? board->whiteBishops : board->blackBishops) >= 2){
  //   mgScore += 22;
  //   egScore += 87;
  // }
  // if (hammingWeight(board->color == BLACK ? board->whiteBishops : board->blackBishops) >= 2){
  //   mgScore -= 22;
  //   egScore -= 87;
  // }
  //-------------
  
  score = (mgScore * phaseMG + egScore * phaseEG) / 24;
  return score;
#else
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

    float whiteMaterial = whitePawns * PAWN_MATERIAL_VALUE + whiteKnights * KNIGHT_MATERIAL_VALUE +
                          whiteBishops * BISHOP_MATERIAL_VALUE + whiteRooks * ROOK_MATERIAL_VALUE +
                          whiteQueens * QUEEN_MATERIAL_VALUE;

    float blackMaterial = blackPawns * PAWN_MATERIAL_VALUE + blackKnights * KNIGHT_MATERIAL_VALUE +
                          blackBishops * BISHOP_MATERIAL_VALUE + blackRooks * ROOK_MATERIAL_VALUE +
                          blackQueens * QUEEN_MATERIAL_VALUE;
    float MD = whiteMaterial - blackMaterial;
    MD = MD < 0 ? -MD : MD;
    float PA = whiteMaterial > blackMaterial ? whitePawns : blackPawns;
    float MS = std::min(2400.0f, MD) + (MD * PA * (8000 - (whiteMaterial + blackMaterial))) / (6400 * (PA + 1));
    MS = std::min(3100.0f, MS);
    // return MS * (this->board->color == 1 ? -1 : 1);
    //  White has more material
    if (whiteMaterial > blackMaterial) {
      if (this->board->color == 1) {
        MS *= -1;
      }
    } else {
      if (this->board->color == 0) {
        MS *= -1;
      }
    }
  return MD;
#endif
}

 // cpy = board->color == WHITE ? board->whiteBishops : board->blackBishops;
  // while (cpy != 0) {
  //   uint64_t isolatedRook = cpy & ((~cpy) + 1);
  //   uint8_t from = 63 - __builtin_ctzll(isolatedRook);
  //   uint64_t attacks = board->bishopAttacks(from);
  //   mgScore += bishopMobilityMG[hammingWeight(attacks)];
  //   //egScore += bishopMobilityEG[hammingWeight(attacks)];
  //   cpy &= ~isolatedRook;
  // }
  
  //passedPawn(board->color == WHITE ? board->whitePawns : board->blackPawns, board->color == BLACK ? board->whitePawns : board->blackPawns, mgScore, egScore);
  
  // Double attacks
  // uint64_t doubleAttacks = (board->queenAttackers & board->rookAttackers) | (board->queenAttackers & board->bishopAttackers) | (board->queenAttackers & board->knightAttackers) | (board->queenAttackers & board->pawnAttackers);
  // doubleAttacks |= (board->rookAttackers & board->bishopAttackers) | (board->rookAttackers & board->knightAttackers) | (board->rookAttackers & board->pawnAttackers);
  // doubleAttacks |= (board->bishopAttackers & board->knightAttackers) | (board->bishopAttackers & board->pawnAttackers);
  // doubleAttacks |= (board->knightAttackers & board->pawnAttackers);
 
  
  // Weak squares
  // uint64_t weak = 0ULL;
  // uint64_t queenAttacks = 0ULL;
  // cpy = board->color == WHITE ? board->whiteBishops : board->blackBishops;
  // while (cpy != 0) {
  //   uint64_t isolatedRook = cpy & ((~cpy) + 1);
  //   uint8_t from = 63 - __builtin_ctzll(isolatedRook);
  //   queenAttacks |= board->bishopAttacks(from) | board->rookAttacks(from);
  //   cpy &= ~isolatedRook;
  // }
  // //flankDefense += hammingWeight(queenAttacks & kingFlank);

  // weak = attackers & (queenAttacks | kingArea);
  // kingDanger += 183 * hammingWeight(weak & kingArea);
  //kingDanger -= 6 * mgScore / 8 + 37;
  //kingDanger -= 1.5 * flankDefense;
  //kingDanger -= 4 * (hammingWeight(attackers & kingFlank) + hammingWeight(attackers & kingFlank & doubleAttacks));

  // mgScore -= kingDanger * kingDanger / 4096;
  // egScore -= kingDanger/16;

  // if (hammingWeight(board->color == WHITE ? board->whiteBishops : board->blackBishops) == 2){
  //   mgScore += 33;
  //   egScore += 65;
  // }
  // if (hammingWeight(board->color == BLACK ? board->whiteBishops : board->blackBishops) == 2){
  //   mgScore -= 33;
  //   egScore -= 65;
  // }