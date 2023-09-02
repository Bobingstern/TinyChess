#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>


int Board::generateMoves(uint16_t* moves){
    int i = 0;
    //printMove(moves[0]);
    i = pawnMoves(color, moves, i);
    //std::cout << i << "\n";
    i = rookMoves(color, moves, i);
    i = knightMoves(color, moves, i);
    i = bishopMoves(color, moves, i);
    i = queenMoves(color, moves, i);
    i = kingMoves(color, moves, i);
    return i;
}

void Board::makeMove(uint16_t a){
    uint8_t from = (a & 0b0000000000111111);
    uint8_t to = ((a >> 6) & 0b0000000000111111);
    uint8_t flag = ((a >> 12) & 0b1111);
    uint64_t fromBitboard = 1ULL << (63 - from);
    uint64_t toBitboard = 1ULL << (63 - to);
    bool isCapture = (flag == 0b0100 || flag == 0b0101 || flag >= 0b1100);
    // Move piece to position first
    // First iterate over all 12 piece bitboards and select it based on from and to
    for (int i=0;i<12;i++){
        // This means a piece is present at 'from'
        //printBitBoard(fromBitboard);
        // Check if it's a capture
        if (isCapture){
            // Remove the piece at 'to' square
            // Remember what we captured
            if ((*bitboards[i] & toBitboard) != 0){
                *bitboards[i] &= (~toBitboard);
                captures.push_back(i);    
            }
                   
        }
        if ((*bitboards[i] & fromBitboard) != 0){
            // Remove the piece abd replace it to its new position
            *bitboards[i] &= (~fromBitboard);
            *bitboards[i] |= toBitboard;
        }
        
        
    }
    //std::cout <<  color <<" ";
    color = !color;
    //std::cout <<  color <<"\n";
}

void Board::unmakeMove(uint16_t a){
    uint8_t from = (a & 0b0000000000111111);
    uint8_t to = ((a >> 6) & 0b0000000000111111);
    uint8_t flag = ((a >> 12) & 0b1111);
    uint64_t fromBitboard = 1ULL << (63 - from);
    uint64_t toBitboard = 1ULL << (63 - to);
    bool isCapture = (flag == 0b0100 || flag == 0b0101 || flag >= 0b1100);
    // Move piece to position first
    // First iterate over all 12 piece bitboards and select it based on from and to
    for (int i=0;i<12;i++){
        // Remove the piece and move back
        if ((*bitboards[i] & toBitboard) != 0){
            // Remove the piece abd replace it to its new position
            *bitboards[i] &= (~toBitboard);
            *bitboards[i] |= fromBitboard;
        }
        
    }
    if (isCapture){
        *bitboards[captures[captures.size()-1]] |= toBitboard; 
        captures.pop_back();           
    }
    color = !color;
}

