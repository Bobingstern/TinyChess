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

int Board::movesFromIndex(int i, uint16_t* moves){
    if (i == 0) return pawnMoves(0, moves, 0);
    if (i == 1) return rookMoves(0, moves, 0);
    if (i == 2) return knightMoves(0, moves, 0);
    if (i == 3) return bishopMoves(0, moves, 0);
    if (i == 4) return queenMoves(0, moves, 0);
    if (i == 5) return kingMoves(0, moves, 0);

    if (i == 6) return pawnMoves(1, moves, 0);
    if (i == 7) return rookMoves(1, moves, 0);
    if (i == 8) return knightMoves(1, moves, 0);
    if (i == 9) return bishopMoves(1, moves, 0);
    if (i == 10) return queenMoves(1, moves, 0);
    if (i == 11) return kingMoves(1, moves, 0);
}
bool Board::isLegal(){
    // color 0 is white
    uint64_t isolated = color == 1 ? whiteKing : blackKing;
    uint8_t from = 63 - __builtin_ctzll(isolated);
    uint64_t attacks;
    // Rook attacks
    attacks = rookAttacks(from);
    if ((attacks & (color == 1 ? blackRooks : whiteRooks)) != 0 ||
        (attacks & (color == 1 ? blackQueens : whiteQueens)) ){
        return false;
    }
    // Bishops
    attacks = bishopAttacks(from);
    if ((attacks & (color == 1 ? blackBishops : whiteBishops)) != 0 ||
        (attacks & (color == 1 ? blackQueens : whiteQueens)) ){
        return false;
    }
    // Knights
    attacks = knightAttacks(isolated, from);
    if ((attacks & (color == 1 ? blackKnights : whiteKnights)) != 0) {
        return false;
    }
    // Pawns
    attacks = 0;
    if (color == 1){
        if ( from % 8 != 0 )
            attacks |= isolated << 9;
        if ((from+1) % 8 != 0)
            attacks |= isolated << 7;
    }
    else {
        if ( (from + 1) % 8 != 0 )
            attacks |= isolated >> 9;
        if (from % 8 != 0)
            attacks |= isolated >> 7;
    }
    if ((attacks & (color == 1 ? blackPawns : whitePawns)) != 0) {
        return false;
    }
    uint16_t flag = (previousMoves[depth-1] & 0b1111000000000000) >> 12;
    bool isCapture = (flag == 0b0100 || flag >= 0b1100);
    bool isEP = (flag == 0b0101);
    // if (isCapture || isEP){
    //     caps++;
    //     std::cout << caps << "\n";
    // }
    return true;
}

void Board::makeMove(uint16_t a){
    //printMove(a);
    previousMoves[depth] = a;
    uint8_t from = (a & 0b0000000000111111);
    
    uint8_t to = ((a >> 6) & 0b0000000000111111);
    uint16_t flag = (a & 0b1111000000000000) >> 12;
    
    uint64_t fromBitboard = 1ULL << (63 - from);
    uint64_t toBitboard = 1ULL << (63 - to);
    bool isCapture = (flag == 0b0100 || flag >= 0b1100);
    bool isEP = (flag == 0b0101);
    // if (from == 31){
    //     printMove(a);
    //     std::cout << isEP << "\n";
    // }
    // Move piece to position first
    // First iterate over all 12 piece bitboards and select it based on from and to
    //std::cout << isEP <<" Yes enp\n";
    if (isEP){
        // epN ++ ;
        // std::cout << epN << "\n";
        if ((*bitboards[  color == 0 ? 6 : 0   ] & (  color == 0 ? (toBitboard >> 8) : toBitboard << 8  ) ) != 0){
            *bitboards[color == 0 ? 6 : 0] &= (~(  color == 0 ? (toBitboard >> 8) : toBitboard << 8  ));
            captures[currentCapture] = color == 0 ? 6 : 0;
            // printBitBoard(whitePawns);
            // printBitBoard(blackPawns);
            currentCapture ++;    
        }
        
    }
    for (int i=0;i<12;i++){
        // This means a piece is present at 'from'
        //printBitBoard(fromBitboard);
        // Check if it's a capture
        if (isCapture){
            // Remove the piece at 'to' square
            // Remember what we captured
            if ((*bitboards[i] & toBitboard) != 0){
                *bitboards[i] &= (~toBitboard);
                captures[currentCapture] = i;
                //std::cout << captures[currentCapture] <<  "\n";
                currentCapture ++;    
            }
                   
        }
        
        if ((*bitboards[i] & fromBitboard) != 0){
            // Remove the piece abd replace it to its new position
            *bitboards[i] &= (~fromBitboard);
            *bitboards[i] |= toBitboard;
            previousMover[depth] = i;
        }
        
        
    }
    
    //std::cout <<  color <<" ";
    color = !color;
    //std::cout <<  color <<"\n";
    depth ++ ;
}

void Board::unmakeMove(uint16_t a){
    depth -- ;
    previousMoves[depth] = 0;
    uint8_t from = (a & 0b0000000000111111);
    uint8_t to = ((a >> 6) & 0b0000000000111111);
    uint8_t flag = ((a >> 12) & 0b1111);
    uint64_t fromBitboard = 1ULL << (63 - from);
    uint64_t toBitboard = 1ULL << (63 - to);
    bool isCapture = (flag == 0b0100 || flag >= 0b1100);
    bool isEP = (flag == 0b0101);
    // Move piece to position first
    // First iterate over all 12 piece bitboards and select it based on from and to
    // Remove the piece and move back
    if ((*bitboards[previousMover[depth]] & toBitboard) != 0){
        // Remove the piece abd replace it to its new position
        *bitboards[previousMover[depth]] &= (~toBitboard);
        *bitboards[previousMover[depth]] |= fromBitboard;
    }
        
    
    if (isCapture){
        *bitboards[captures[currentCapture - 1]] |= toBitboard; 
        captures[currentCapture - 1] = -1;
        currentCapture -- ;          
    }
    if (isEP){
        //std::cout << "EP\n";
        *bitboards[color == 1 ? 6 : 0] |= ((  color == 1 ? (toBitboard >> 8) : toBitboard << 8  ));
        captures[currentCapture] = -1;
        // printBitBoard(whitePawns);
        // printBitBoard(blackPawns);
        currentCapture --;   
        
    }
    previousMover[depth] = -1;
    color = !color;
    
}

