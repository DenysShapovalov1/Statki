#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include "Ship.h"

class Board {
public:
    Board();
    void resetShotHistory();
    void displayBoard() const;
    void displayBoards(const Board& opponentBoard) const;
    bool canPlaceShip(int x, int y, int size, bool horizontal, std::string& errorMessage);
    bool placeShip(int x, int y, int size, const std::string& name, bool horizontal);
    bool shoot(int x, int y);
    bool allShipsSunk() const;
    bool shotHistory[10][10];

private:
    char board[10][10];
    std::vector<Ship> ships; 
    int shipCount;

    bool isShipSunk(int x, int y);
    int findShipIndex(int x, int y);
    void markSurroundingCellsAsMissed(int shipIndex);
};

#endif 
