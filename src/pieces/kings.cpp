#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

uint64_t Board::kingAttacks(uint64_t isolated, uint8_t from){
    uint64_t attacks;
    attacks |= isolated << 8;
    attacks |= isolated >> 8;
    if ((from) % 8 != 0){
        // Not on A file
        attacks |= isolated << 1;
        attacks |= isolated >> 7;
        attacks |= isolated << 7;
    }
    if ((from+1) % 8 != 0){
        // Not on A file
        attacks |= isolated >> 1;
        attacks |= isolated >> 9;
        attacks |= isolated << 9;
    }
    return attacks;
}

std::vector<uint16_t> Board::kingMoves(bool color){

    std::vector<uint16_t> moves;
    uint64_t kingsCopy = color == 0 ? whiteKing : blackKing;
    
    while (kingsCopy != 0){
        uint64_t isolatedKing = kingsCopy & ((~kingsCopy)+1);
        uint8_t from = 63 - __builtin_ctzll(isolatedKing);
        uint64_t attacks = kingAttacks(isolatedKing, from);
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
        kingsCopy &= ~isolatedKing;
    }
    return moves;
}