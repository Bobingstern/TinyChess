#include "board/board.h"
#include "engine/engine.h"
#include <bitset>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

const uint16_t BOOK_MOVES[] = {
    //    movePack(A2, A4, true, false, 0, false, 0) // 1-0 in 21
    // Queen's gambit accepted
    movePack(D2, D4, true, false, 0, false, 0), movePack(D7, D5, true, false, 0, false, 0),
    movePack(C2, C4, true, false, 0, false, 0), movePack(D5, C4, false, false, 0, true, 0),
    movePack(E2, E4, true, false, 0, false, 0)};
const uint8_t BOOK_MOVE_LEN = sizeof(BOOK_MOVES) / sizeof(uint16_t);

#define PRINT_PGN_ONLY

// SPRT test on cutechess CLI for TinyChess0 and TinyChess1
// cutechess-cli.exe -engine conf=TinyChess1 -engine conf=TinyChess0 -each proto=uci tc=inf -sprt elo0=0 elo1=5
// alpha=0.05 beta=0.05 -games 200 -openings file=Silver_Suite.pgn format=pgn plies=5

int main() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Board board = Board();

  Engine engine = Engine(&board);

  std::string Line; // to read the command given by the GUI

  std::cout.setf(std::ios::unitbuf); // Make sure that the outputs are sent straight away to the GUI

  while (std::getline(std::cin, Line)) {
    if (Line == "uci") {
      std::cout << "id name TinyChess" << std::endl;
      std::cout << "id author Anik Patel and Cyrus Yiu" << std::endl;
      std::cout << "uciok" << std::endl;
    } else if (Line == "quit") {
      std::cout << "Bye Bye" << std::endl;
      break;
    } else if (Line == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (Line == "ucinewgame") {
      board.readFEN("position startpos");
    }
    if (Line.substr(0, 8) == "position") {
      board.readFEN(Line);
    } else if (Line == "stop") {
      return 0;
    } else if (Line.substr(0, 3) == "go ") {
      uint16_t m;
      if (Line.substr(3, Line.length()) == "infinite") {
        m = engine.runSearch(4, -1);
      } else {
        m = engine.runSearch(6, std::stoi(Line.substr(12, Line.length())));
      }
      std::cout << "bestmove ";
      board.printMove(m);
      std::cout << "\n";
    }
  }

  //   // Mate testing
  //   for (int i = 0; i < 120; i++) {
  // #ifndef PRINT_PGN_ONLY
  //     std::cout << "Move " << i << "\n";
  // #endif
  //     auto t1 = high_resolution_clock::now();
  //     bool done = false;
  //     uint16_t bestMove;
  //     if (i <= BOOK_MOVE_LEN - 1) {
  //       bestMove = BOOK_MOVES[i];
  //     }
  //     else {
  //       bestMove = engine.runSearch(4, done);
  //     }
  //     if (done) {
  //       break;
  //     }
  //     auto t2 = high_resolution_clock::now();
  //     duration<double, std::milli> ms_double = t2 - t1;
  // #ifdef PRINT_PGN_ONLY
  //     if (i % 2 == 0) {
  //       if (i % 8 == 0) {
  //         std::cout << "\n";
  //       }
  //       std::cout << (i / 2) + 1 << ". ";
  //     }
  //     board.printMove(bestMove);
  //     std::cout << " ";
  // #else
  //     std::cout << ((double)(ms_double.count()) / 1000.0) << " seconds.\nEval: ";
  //     std::cout << engine.staticEvaluation();
  //     std::cout << "\nBest move: ";
  //     board.printMove(bestMove);
  //     std::cout << "\n";
  // #endif
  //     std::fill_n(board.captures, 128, 0);
  //     std::fill_n(board.previousMover, 128, 0);
  //     std::fill_n(board.previousMoves, 128, 0);
  //     board.depth = 0;
  //     board.currentCapture = 0;
  //     board.makeMove(bestMove);

  // #ifndef PRINT_PGN_ONLY
  //     board.printBoard();
  // #endif
  //   }
  return 0;
}