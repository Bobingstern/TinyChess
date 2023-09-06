#include "../board.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>


int Board::generateMoves(uint16_t* moves, uint64_t &pawnAttacks, uint64_t &rookAttacks, uint64_t &knightAttacks, uint64_t &bishopAttacks, uint64_t &queenAttacks, uint64_t &kingAttacks){
    int i = 0;
    // uint64_t opposingAttacks = pawnAttackers | rookAttackers | knightAttackers | bishopAttackers | queenAttackers | kingAttackers;
    //std::cout << color << "\n";
    if (color == 0){
        // Checking for checks in castling squares
        if (flagWhiteKingsideCastle){
            // TODO
            // Make sure no piece "checks" the squares the king is moving thorugh during castle
            if ( (combinedOccupation() & 6ULL) == 0){
                //printBoard();
                moves[i] = 0b0010111110111100;
                i++;
            }
        }
        // if (flagWhiteQueensideCastle){
        //     if ((opposingAttacks & 112ULL) == 0 && (combinedOccupation() & 112ULL) == 0){
        //         moves[i] = 0b0011000010001000;
        //         i++;
        //     }
        // }
    }
    
    
    //printMove(moves[0]);
    resetAttackers();
    i = pawnMoves(color, moves, i);
    //std::cout << i << "\n";
    i = rookMoves(color, moves, i);
    i = knightMoves(color, moves, i);
    i = bishopMoves(color, moves, i);
    i = queenMoves(color, moves, i);
    i = kingMoves(color, moves, i);
    

    // Totally not confusing
    pawnAttacks = pawnAttackers;
    rookAttacks = rookAttackers;
    knightAttacks = knightAttackers;
    bishopAttacks = bishopAttackers;
    queenAttacks = queenAttackers;
    kingAttacks = kingAttackers;
    // if (queenAttackers != 0)
    //     printBitBoard(queenAttackers);
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

void Board::resetAttackers(){
    pawnAttackers = 0ULL;
    rookAttackers = 0ULL;
    knightAttackers = 0ULL;
    bishopAttackers = 0ULL;
    queenAttackers = 0ULL;
    kingAttackers = 0ULL;
}

void Board::setAttackers(uint64_t pawnAttacks, uint64_t rookAttacks, uint64_t knightAttacks, uint64_t bishopAttacks, uint64_t queenAttacks, uint64_t kingAttacks){
    pawnAttackers = pawnAttacks;
    rookAttackers = rookAttacks;
    knightAttackers = knightAttacks;
    bishopAttackers = bishopAttacks;
    queenAttackers = queenAttacks;
    kingAttackers = kingAttacks;
}

uint64_t Board::getAttackers(){
    return pawnAttackers | rookAttackers | knightAttackers  | bishopAttackers | queenAttackers |kingAttackers;
}
bool Board::isLegal(uint64_t& attackers){
    // color 0 is white
    uint16_t flag = (previousMoves[depth-1] & 0b1111000000000000) >> 12;
    if (flag == 0b0010){
        //std::cout << color << "\n";
        
        //printBitBoard(blackBishops);
        if (color == 1){
            if ((14ULL & attackers) != 0){
                //printBoard();
                // printBitBoard(attackers);
                // printBoard();
                return false;
            }
            else {
                return true;
            }
        }
    }
    uint64_t isolated = color == 1 ? whiteKing : blackKing;
    uint8_t from = 63 - __builtin_ctzll(isolated);
    uint64_t attacks;

    if ((previousMover[depth-1] == 5 || previousMover[depth-1] == 11)){
        // Was a king move and we can use cached vision data
        if ((attackers & isolated) != 0){
            //std::cout << color << "\n";
            //printBitBoard(attackers);
            //printBoard();
            
            return false;
        }
    }
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
    // uint16_t flag = (previousMoves[depth-1] & 0b1111000000000000) >> 12;
    // bool isCapture = (flag == 0b0100 || flag >= 0b1100);
    // bool isEP = (flag == 0b0101);
    // if (isCapture || isEP){
    //     caps++;
    //     std::cout << caps << "\n";
    // }
    return true;
}
void Board::sliceReadd(){
    // Slice the mover ray and re add
    uint64_t prevTo = 1ULL << (63-((previousMoves[depth] & 0b0000111111000000) >> 6));

    // if (previousMoves[depth] == 0b0000100110101110){
    //     printBitBoard(bishopAttackers);
    //     printBitBoard(prevTo);
    // }
    if (previousMover[depth] == 3 || previousMover[depth] == 9 || (bishopAttackers & prevTo) != 0){
        bishopAttackers = 0;
        // Regenerate bishops only
        uint64_t bishopsCopy = color == 0 ? whiteBishops : blackBishops;
        while (bishopsCopy != 0){
            uint64_t isolatedBishop = bishopsCopy & ((~bishopsCopy)+1);
            uint8_t from = 63 - __builtin_ctzll(isolatedBishop);
            uint64_t attacks = bishopAttacks(from);
            bishopAttackers |= attacks;
            bishopsCopy &= ~isolatedBishop;
        }
    }
    // Rook
    if (previousMover[depth] == 1 || previousMover[depth] == 7 || (rookAttackers & prevTo) != 0){
        rookAttackers = 0;
        // Regenerate bishops only
        uint64_t rooksCopy = color == 0 ? whiteRooks : blackRooks;
        while (rooksCopy != 0){
            uint64_t isolatedRook = rooksCopy & ((~rooksCopy)+1);
            uint8_t from = 63 - __builtin_ctzll(isolatedRook);
            uint64_t attacks = rookAttacks(from);
            rookAttackers |= attacks;
            rooksCopy &= ~isolatedRook;
        }
    }
    // Queen
    if (previousMover[depth] == 4 || previousMover[depth] == 10 || (queenAttackers & prevTo) != 0){
        queenAttackers = 0;
        // Regenerate bishops only
        uint64_t queensCopy = color == 0 ? whiteQueens : blackQueens;
        while (queensCopy != 0){
            uint64_t isolatedQueen = queensCopy & ((~queensCopy)+1);
            uint8_t from = 63 - __builtin_ctzll(isolatedQueen);
            uint64_t attacks = rookAttacks(from) | bishopAttacks(from);
            queenAttackers |= attacks;
            queensCopy &= ~isolatedQueen;
        }
    }
    // Pawn
    if (previousMover[depth] == 0 || previousMover[depth] == 6 || (pawnAttackers & prevTo) != 0){
        pawnAttackers = 0;
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
            pawnsCopy &= ~isolatedPawn;
        }
    }

    if (previousMover[depth] == 5 || previousMover[depth] == 11){
        kingAttackers = 0;
        uint64_t kingsCopy = color == 0 ? whiteKing : blackKing;
        while (kingsCopy != 0){
            uint64_t isolatedKing = kingsCopy & ((~kingsCopy)+1);
            uint8_t from = 63 - __builtin_ctzll(isolatedKing);
            uint64_t attacks = kingAttacks(isolatedKing, from);
            //printBitBoard(kingAttacks(isolatedKing, from));
            kingAttackers |= attacks;
            kingsCopy &= ~isolatedKing;
        }
    }

    if (previousMover[depth] == 2 || previousMover[depth] == 8){
        knightAttackers = 0;
        uint64_t knightsCopy = color == 0 ? whiteKnights : blackKnights;
        while (knightsCopy != 0){
            uint64_t isolatedKnight = knightsCopy & ((~knightsCopy)+1);
            uint8_t from = 63 - __builtin_ctzll(isolatedKnight);
            uint64_t attacks = knightAttacks(isolatedKnight, from);
            //printBitBoard(attacks);
            knightAttackers |= attacks;
            knightsCopy &= ~isolatedKnight;
        }
    }
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

    if (flag == 0b0010){
        //printBoard();
        if (color == 0){
            whiteKing >>= 2;
            whiteRooks &= (~1ULL);
            whiteRooks |= (whiteKing << 1);
            flagWhiteKingsideCastle = 0;
            wkscDepth = depth;
        }
        color = !color;
        previousMover[depth] = 5;
        depth ++ ;
        
        return;
    }
    if (flag == 0b0011){
        if (color == 0){
            whiteKing <<= 2;
            whiteRooks &= (~128ULL);
            whiteRooks |= (whiteKing >> 1);
            flagWhiteQueensideCastle = 0;
            wqscDepth = depth;
        }
        color = !color;
        previousMover[depth] = 5;
        depth ++ ;
        //printBoard();
        return;
    }
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
    if (color == 0){
        // Right white rook has moved/been captured
        // White King moved
        if ( ((whiteKing & 8ULL) == 0 || (whiteRooks & 1ULL) == 0) && 
            !flagWhiteKingsideCastle){
            // Trip the bit
            wkscDepth = depth;
            flagWhiteKingsideCastle = 0; // Set the flag to unable to castle
        }
        // Queenside castle
        if ( ((whiteKing & 8ULL) == 0 || (whiteRooks & 128ULL) == 0) && 
            !flagWhiteQueensideCastle){
            // Trip the bit
            wqscDepth = depth;
            flagWhiteQueensideCastle = 0; // Set the flag to unable to castle
        }
    }
    
    sliceReadd();
    
    
    //std::cout <<  color <<" ";
    color = !color;
    //std::cout <<  color <<"\n";
    depth ++ ;
}

void Board::unmakeMove(uint16_t a){
    depth -- ;
    previousMoves[depth] = 0;
    if (depth == wkscDepth){
        // Weve reached the depth where the castling flag
        // was tripped and so we undo it since we have gone back to the move
        flagWhiteKingsideCastle = 1;
        wkscDepth = -1;
    }
    if (depth == wqscDepth){
        flagWhiteQueensideCastle = 1;
        wqscDepth = -1;
    }

    uint8_t from = (a & 0b0000000000111111);
    uint8_t to = ((a >> 6) & 0b0000000000111111);
    uint8_t flag = ((a >> 12) & 0b1111);
    uint64_t fromBitboard = 1ULL << (63 - from);
    uint64_t toBitboard = 1ULL << (63 - to);
    bool isCapture = (flag == 0b0100 || flag >= 0b1100);
    bool isEP = (flag == 0b0101);

    if (flag == 0b0010){
        if (color == 1){
            whiteKing <<= 2;
            whiteRooks &= ~(whiteKing >> 1);
            whiteRooks |= 1ULL;
            flagWhiteKingsideCastle = 1;
            //std::cout << "Unm,aked\n";
            wkscDepth = -1;
        } 
        previousMover[depth] = -1;
        color = !color;
        return;
    }
    if (flag == 0b0011){
        //printBoard();
        if (color == 1){
            whiteKing >>= 2;
            whiteRooks &= ~(whiteKing >> 1);
            whiteRooks |= (128ULL);
            flagWhiteQueensideCastle = 1;
            wqscDepth = -1;
        }
        previousMover[depth] = -1;
        color = !color;
        return;
    }


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
    sliceReadd();
    previousMover[depth] = -1;
    color = !color;
    
}

