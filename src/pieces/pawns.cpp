#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

// Move order

int Board::pawnMoves(bool color, uint16_t* moves, int i){
    // Captures
    uint64_t pawnsCopy = color == 0 ? whitePawns : blackPawns;
    while (pawnsCopy != 0){
        uint64_t isolatedPawn = pawnsCopy & ((~pawnsCopy)+1);
        int from = 63 - __builtin_ctzll(isolatedPawn);
        uint64_t movements = 0;
        // Attacks/Captures
        // Check color and generate diagonal attack square
        if (color == 0){
            if ( from % 8 != 0 )
                movements |= isolatedPawn << 9;
            if ((from+1) % 8 != 0)
                movements |= isolatedPawn << 7;
        }
        else {
            if ( (from + 1) % 8 != 0 )
                movements |= isolatedPawn >> 9;
            if (from % 8 != 0)
                movements |= isolatedPawn >> 7;
        }
        // Iterate over attack squares and see if the move is valid (capturable piece)
        pawnAttackers |= movements;
        while (movements != 0){
            uint64_t isolatedAttack = movements & ((~movements)+1);
            int to = 63 - __builtin_ctzll(isolatedAttack);
            if (( (color == 0 ? blackOccupation() : whiteOccupation()) & isolatedAttack ) != 0){
                
                moves[i] = movePack(from, to, false, false, false, true, 0);
                i++;
            }
            // En Passant moves
            if (color == 0){
                if (depth > 0 && previousMover[depth-1] == 6){
                    if ((from >= 24 && from < 32) && 
                        ((previousMoves[depth - 1] & 0b1111000000000000) >> 12 ) == 1 &&
                        ((previousMoves[depth - 1] & 0b0000111111000000) >> 6 ) == to + 8){
                            // std::cout << from << " " << to <<"\n";
                            // printBitBoard(whitePawns);
                            // printBitBoard(blackPawns);
                            // printBoard();
                            // std::cout << "\n";
                            moves[i] = movePack(from, to, false, true, false, false, 0);
                            i++;
                            //printMove(moves[i]);
                    }
                }
            }
            else{
                // if (from >= 32){
                //     std::cout << depth << "\n";
                // }
                    
                
                if (depth > 0 && previousMover[depth-1] == 0){
                    
                    if ((from >= 32 && from < 40) && 
                        ((previousMoves[depth - 1] & 0b1111000000000000) >> 12 ) == 1 &&
                        ((previousMoves[depth - 1] & 0b0000111111000000) >> 6 ) == to - 8){
                            
                            // printBitBoard(whitePawns);
                            // printBitBoard(blackPawns);
                            moves[i] = movePack(from, to, false, true, false, false, 0);
                            i++;
                            
                            //printMove(moves[i]);
                    }
                }
            }
            movements &= ~isolatedAttack;
        }
        // Pushes
        movements = 0;
        if ( color == 0){
            // Make sure nothing is in front of the white pawn
            if ((combinedOccupation() & (isolatedPawn << 8)) == 0){
                movements |= (isolatedPawn << 8);
                // Make sure nothing is in the way of double push
                if (from >= 48 && from <= 55 && (combinedOccupation() & (isolatedPawn << 16)) == 0){
                    movements |= (isolatedPawn << 16);
                }
            }
        }
        else {
            if ((combinedOccupation() & (isolatedPawn >> 8)) == 0){
                movements |= (isolatedPawn >> 8);
                // Make sure nothing is in the way of double push
                if (from >= 8 && from <= 15 && (combinedOccupation() & (isolatedPawn >> 16)) == 0){
                    movements |= (isolatedPawn >> 16);
                }
            }
        }
        
        // Push the moves
        //pawnAttackers |= movements;
        while (movements != 0){
            uint64_t isolatedPush = movements & ((~movements)+1);
            int to = 63 - __builtin_ctzll(isolatedPush);
            if (color == 0){
                // if (to - from > 8){
                //     std::cout << "Flag\n";
                // }
                moves[i] = movePack(from, to, from - to > 8, false, false, false, 0);
            }
            else{
                moves[i] = movePack(from, to, to - from > 8, false, false, false, 0);
            }
            i++;
            movements &= ~isolatedPush;
        }

        pawnsCopy &= ~isolatedPawn;
    }
    return i;

}