#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>


class Board {
protected:
    bool shotHistory[10][10];
private:
    char board[10][10];
    std::string ships[10]; 
    char shipSymbols[10]; 
    int shipPositions[10][5][2];
    bool shipHidden[10][10];
    int shipCount; 
    bool shipSunk[4];
  


    

public:
    Board();
    void displayBoard() const;
    void displayBoards (const Board& opponentBoard) const;
    void hideShips();
    bool placeShip(int x, int y, int size, const std::string& name, bool horizontal);
    bool shoot(int x, int y);
    bool allShipsSunk() const;
    bool canPlaceShip(int x, int y, int size, bool horizontal);
    void resetShotHistory(); 
    void markSurroundingCellsAsMissed(int shipIndex);
    bool isShipSunk(int x, int y);
    int findShipIndex(int x, int y);
};




#endif 