#include <iostream>
#include <bitset>
#include "board.h"

//g++ src/*.cpp src/pieces/*.cpp -o main.exe -std=c++17
int main () {
    Board board = Board();
    board.printBoard();
    
    //board.printAllBitBoards();
    std::vector<uint16_t> moves = board.pawnMoves(0);
    for (int i=0;i<moves.size();i++){
        board.printMove(moves[i]);
        board.makeMove(moves[i]);
    }
    board.printBoard();
    return 0;
}