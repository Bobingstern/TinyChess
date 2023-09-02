#include <iostream>
#include <bitset>
#include "board.h"
#include <chrono>
#include <stdio.h>
#include <string.h>
#include <vector>

//g++ src/*.cpp src/pieces/*.cpp -o main.exe -std=c++17
uint64_t perft(Board &b, int depth){
    
    if (depth == 0){
        return 1ULL;
    }
    uint16_t moves[218];
    //memset(moves, 0, sizeof(moves));
    //std::cout << b.color << " ";
    int total = b.generateMoves(moves);
    uint64_t nodes = 0;
    //std::cout << b.color << " \n";
    //b.printAllBitBoards();
    
    for (int i=0;i<total;i++){
        //b.printMove(moves[i]);
        // if (moves[i] == 0)
        //     break;
        b.makeMove(moves[i]);
        //b.printBoard();
        //std::cout << "\n";
        nodes += perft(b, depth-1);
        b.unmakeMove(moves[i]);
        
    }
    return nodes;
    // static uint16_t moves[218];
    // memset(moves, 0, sizeof(moves));
    // //std::cout << b.color << "\n";
    // b.generateMoves(moves);
    // for (int i=0;i<218;i++){
    //     //b.printMove(moves[i]);
    //     if (moves[i] == 0)
    //         break;
    //     b.makeMove(moves[i]);
    //     //b.printBoard();
    //     b.unmakeMove(moves[i]);
    //     //std::cout << "\n";
    //     //board.printBoard();
    // }
    // return 0;
}

int main () {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    Board board = Board();
    board.printBoard();

    auto t1 = high_resolution_clock::now();
    uint64_t perftNodes = perft(board, 6);
    auto t2 = high_resolution_clock::now();
    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "Nodes:" << perftNodes << " in "<< ms_double.count() << "ms\n";
    std::cout << (double)(perftNodes) / ((double)(ms_double.count()) / 1000.0) << " NPS";
    //board.printAllBitBoards();
    // static uint16_t moves[218];
    // memset(moves, 0, sizeof(moves));
    // board.generateMoves(moves);
    // for (int i=0;i<218;i++){
    //     board.printMove(moves[i]);
    //     if (moves[i] == 0)
    //         break;
    //     board.makeMove(moves[i]);
    //     board.printBoard();
    //     board.unmakeMove(moves[i]);
    //     std::cout << "\n";
    //     //board.printBoard();
    // }
    
    return 0;
}