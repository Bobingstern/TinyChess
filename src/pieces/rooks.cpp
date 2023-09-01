#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

uint64_t Board::rookAttacks(uint8_t from){
    return rankAttacks(from) | fileAttacks(from);
}

std::vector<uint16_t> Board::rookMoves(bool color){

    std::vector<uint16_t> moves;
    uint64_t rooksCopy = color == 0 ? whiteRooks : blackRooks;
    
    while (rooksCopy != 0){
        uint64_t isolatedRook = rooksCopy & ((~rooksCopy)+1);
        uint8_t from = 63 - __builtin_ctzll(isolatedRook);
        uint64_t attacks = rookAttacks(from);
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
        rooksCopy &= ~isolatedRook;
    }
    return moves;
}