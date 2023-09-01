#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

uint64_t Board::bishopAttacks(uint8_t from){
    int diag = 7 - ((63 - from) & 7) - ((63 - from) >> 3);
    uint64_t positiveDiagonalRay = diag >= 0 ? 0x0102040810204080ULL >> diag * 8 : 0x0102040810204080ULL << -diag * 8;
    diag = ((63 - from) & 7) - ((63 - from) >> 3);
    uint64_t negativeDiagonalRay = diag >= 0 ? 0x8040201008040201ULL  >> diag * 8 : 0x8040201008040201ULL  << -diag * 8;
    //printBitBoard(positiveDiagonalRay);
    return obstructionDifference(from, positiveDiagonalRay) | obstructionDifference(from, negativeDiagonalRay);
}

std::vector<uint16_t> Board::bishopMoves(bool color){

    std::vector<uint16_t> moves;
    uint64_t bishopsCopy = color == 0 ? whiteBishops : blackBishops;
    
    while (bishopsCopy != 0){
        uint64_t isolatedBishop = bishopsCopy & ((~bishopsCopy)+1);
        uint8_t from = 63 - __builtin_ctzll(isolatedBishop);
        uint64_t attacks = bishopAttacks(from);
        //printBitBoard(attacks);
        while (attacks != 0){
            uint64_t isolatedAttack = attacks & ((~attacks)+1);
            uint8_t to = 63 - __builtin_ctzll(isolatedAttack);
            // tis a capture of the opposing color
            if (((color == 0 ? blackOccupation() : whiteOccupation()) & isolatedAttack ) != 0){
                moves.push_back(movePack(from, to, false, false, false, true, 0));
            }
            else if ( ((color == 0 ? whiteOccupation() : blackOccupation()) & isolatedAttack ) == 0 ){
                // No capture and not our own color
                moves.push_back(movePack(from, to, false, false, false, false, 0));
            }
            attacks &= ~isolatedAttack;
        }
        bishopsCopy &= ~isolatedBishop;
    }
    return moves;
}