#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

uint64_t Board::whiteOccupation(){
    return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
}

uint64_t Board::blackOccupation(){
    return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
}

uint64_t Board::combinedOccupation(){
    return whiteOccupation() | blackOccupation();
}

void Board::printMove(uint16_t a){
    std::bitset<16> m = a;
    std::bitset<4> flag = a >> 12;
    std::cout << "Binary Move:" << m << "\n";
    std::cout << "From:" << (a & 0b0000000000111111) << "\n";
    std::cout << "To:" << ((a & 0b0000111111000000) >> 6) << "\n";
    std::cout << "Flag:" << flag << "\n";
}
