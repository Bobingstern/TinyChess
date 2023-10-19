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
      std::cout << "id name TinyChess";
  #ifndef USE_PST
      std::cout << "-nopst";
  #endif
      std::cout << std::endl;
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
    else if (Line == "info"){
      std::cout << "Board Size: " << sizeof(board) << "\n";
      std::cout << "Engine Size: " << sizeof(engine) << "\n";
      std::cout << "Total Size: " << sizeof(engine) + sizeof(board) << "\n";
    }
    else if (Line.substr(0, 5) == "perft") {
      engine.runPerft(std::stoi(Line.substr(6, 7)));
    }
    if (Line.substr(0, 8) == "position") {
      board.readFEN(Line);
    } else if (Line == "stop") {
      return 0;
    } else if (Line == "eval") {
        uint64_t dum = 0;
        std::cout << engine.getEval() << "\n";
    // } else if (Line.substr(0, 5) == "tune "){
    //   engine.evalScoreTuner(Line);
    // }
    
    } else if (Line.substr(0, 3) == "go ") {
      uint16_t m;
      int sc = 0;
      int nodes = 0;
      if (Line.substr(3, Line.length()) == "infinite") {
        m = engine.runSearch(6, -1);
      } else {
        
        if (Line.substr(3, 9) == "movetime "){
          int end = -1;
          Line = Line.substr(12, Line.length());
          int T = std::stoi(Line);
          // int nodes = -1;
          // if (Line.substr(end, 6) == " nodes"){
          //   nodes = std::stoi(Line.substr(end+6, Line.length()));
          //   std::cout << nodes << "\n";
          // }
          m = engine.runSearchID(T, sc, nodes);
        }
        
        else if (Line.substr(3, 6) == "wtime "){
          int end = -1;
          Line = Line.substr(9, Line.length());
          for (int i=0;i<Line.length();i++){
            if (Line[i] == ' '){
              end = i;
              break;
            }
          }
          int wtime = std::stoi(Line.substr(0, end));
          Line = Line.substr(end+1, Line.length() - end);
          end = -1;
          for (int i=0;i<Line.length();i++){
            if (Line[i] == ' '){
              end = i;
              break;
            }
          }
          int btime = std::stoi(Line.substr(end, Line.length()-end));
          // std::cout << wtime << "\n";
          // std::cout << btime << "\n";
          int timeLeft = board.color == WHITE ? wtime : btime;
          
          timeLeft /= 20;
          m = engine.runSearchID(timeLeft, sc, nodes);
        }
      }
      std::cout << "bestmove ";
      board.printMove(m);
      std::cout << "\n";
    }
  }
  return 0;
}