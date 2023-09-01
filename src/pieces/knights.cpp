#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

uint64_t Board::knightAttacks(uint64_t isolated, uint8_t from){
    uint64_t attacks = 0;
    if (from % 8 != 0) {
        attacks |= isolated << 17;
        attacks |= isolated >> 15;
        // Not A or B file
        if ((from - 1) % 8 != 0) {
            attacks |= isolated << 10;
            attacks |= isolated >> 6;
        }
    }

    // Not H File
    if ((from + 1) % 8 != 0) {
        attacks |= isolated << 15;
        attacks |= isolated >> 17;
        // Not H or F File
        if ((from + 2) % 8 != 0) {
            attacks |= isolated << 6;
            attacks |= isolated >> 10;
        }
    }
    return attacks;
}

std::vector<uint16_t> Board::knightMoves(bool color){

    std::vector<uint16_t> moves;
    uint64_t knightsCopy = color == 0 ? whiteKnights : blackKnights;
    
    while (knightsCopy != 0){
        uint64_t isolatedKnight = knightsCopy & ((~knightsCopy)+1);
        uint8_t from = 63 - __builtin_ctzll(isolatedKnight);
        uint64_t attacks = knightAttacks(isolatedKnight, from);
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
        knightsCopy &= ~isolatedKnight;
    }
    return moves;
}