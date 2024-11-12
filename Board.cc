#include "Board.h"


Board::Board() : shipCount(0) {
    resetShotHistory();
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            board[i][j] = '.';
            shotHistory[i][j] = false;
            shipHidden[i][j] = false;
        }
    }
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 5; ++j) {
            shipPositions[i][j][0] = -1; 
            shipPositions[i][j][1] = -1;
        }
    }

}



void Board::resetShotHistory() {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                shotHistory[i][j] = false; 
            }
        }
    }

void Board::hideShips() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (board[i][j] != 'O' && board[i][j] != 'X') {
                shipHidden[i][j] = true; 
            }
        }
    }
}



void Board::displayBoard() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    std::cout << "  0 1 2 3 4 5 6 7 8 9\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << i << " ";
        for (int j = 0; j < 10; ++j) {
            std::cout << board[i][j] << " ";
        }
        
        std::cout << "\n";
    }
    
}



void Board::displayBoards(const Board& opponentBoard) const {
    std::cout << "  0 1 2 3 4 5 6 7 8 9      0 1 2 3 4 5 6 7 8 9\n";
    
    for (int i = 0; i < 10; ++i) {
        std::cout << i << " ";
        
        
        for (int j = 0; j < 10; ++j) {
            std::cout << board[i][j] << " ";
        }

        std::cout << "   " << i << " ";
        
        
        for (int j = 0; j < 10; ++j) {
            if (opponentBoard.shotHistory[i][j]) { 
                if (opponentBoard.board[i][j] == 'X') {
                    std::cout << "X "; 
                } else {
                    
                    std::cout << "O "; 
                }
            } else {
                std::cout << ". "; 
            }
        }
        std::cout << "\n";
    }
    
}



bool Board::canPlaceShip(int x, int y, int size, bool horizontal) {
    if (horizontal) {
        if (y + size > 10) return false;
    } else {
        if (x + size > 10) return false;
    }

    
    for (int i = -1; i <= size; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = horizontal ? x + j : x + i;
            int newY = horizontal ? y + i : y + j;

            if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10) {
                if (board[newX][newY] != '.') {
                    return false; 
                }
            }
        }
    }
    return true; 
}

bool Board::placeShip(int x, int y, int size, const std::string& name, bool horizontal) { 
    if (!canPlaceShip(x, y, size, horizontal)) {
        return false; 
    }

    shipSymbols[shipCount] = name[0];
    for (int i = 0; i < size; ++i) {
        if (horizontal) {
            board[x][y + i] = name[0];
            shipPositions[shipCount][i][0] = x; 
            shipPositions[shipCount][i][1] = y + i;
            shipHidden[x][y + i] = true;
        } else {
            board[x + i][y] = name[0];
            shipPositions[shipCount][i][0] = x + i; 
            shipPositions[shipCount][i][1] = y;
            shipHidden[x + i][y] = true;
        }
    }

    shipCount++;
    return true;
}



bool Board::shoot(int x, int y) {
    if (shotHistory[x][y]) {
        std::cout << "Już strzelałeś w to miejsce!\n";
        return false; 
    }

    shotHistory[x][y] = true;

    if (board[x][y] == '.') {
        board[x][y] = 'O'; 
        return false; 
    } else if (board[x][y] != 'O' && board[x][y] != 'X') {
        board[x][y] = 'X'; 
        if (isShipSunk(x, y)) {
    std::cout << "Statek jest zatopiony!\n";
    
    
    int shipIndex = findShipIndex(x, y);
    
    
    bool fullyDestroyed = true;
    for (int i = 0; i < 5; ++i) {
        if (shipPositions[shipIndex][i][0] == -1) break;
        
        int shipX = shipPositions[shipIndex][i][0];
        int shipY = shipPositions[shipIndex][i][1];
        

        if (board[shipX][shipY] != 'X') {
            fullyDestroyed = false;
            break;
        }
    }
    
    
    if (fullyDestroyed) {
        markSurroundingCellsAsMissed(shipIndex);
    }
    
    if (allShipsSunk()) {
        std::cout << "Wszystkie statki są zatopione!\n";
        return true; 
    }

}
return true;
    }
    return false; 
}


int Board::findShipIndex(int x, int y) {
    for (int shipIndex = 0; shipIndex < 10; ++shipIndex) {
        for (int i = 0; i < 5; ++i) {
            if (shipPositions[shipIndex][i][0] == x && 
                shipPositions[shipIndex][i][1] == y) {
                return shipIndex;
            }
        }
    }
    return -1; 
}



bool Board::allShipsSunk() const {
    for (int i = 0; i < shipCount; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (shipPositions[i][j][0] == -1) break;
            
            int x = shipPositions[i][j][0];
            int y = shipPositions[i][j][1];
            
            if (board[x][y] != 'X') {
                return false; 
            }
        }
    }
    return true; 
}


bool Board::isShipSunk(int x, int y) {
    for (int i = 0; i < shipCount; ++i) {
        bool isCurrentShipSunk = true;

        for (int j = 0; j < 5; ++j) {
            if (shipPositions[i][j][0] == -1) break; 
            int partX = shipPositions[i][j][0];
            int partY = shipPositions[i][j][1];

            
            if (board[partX][partY] != 'X') {
                isCurrentShipSunk = false;
                break;
            }
        }

        
        if (isCurrentShipSunk) {
            std::cout << "Statek " << i << " zatopiony." << std::endl; 
            markSurroundingCellsAsMissed(i); 
            return true; 
        }
    }
    return false; 
}





void Board::markSurroundingCellsAsMissed(int shipIndex) {
  
    int minX = 10, maxX = -1;
    int minY = 10, maxY = -1;
    
    for (int i = 0; i < 5; ++i) {
        if (shipPositions[shipIndex][i][0] == -1) break;
        
        int x = shipPositions[shipIndex][i][0];
        int y = shipPositions[shipIndex][i][1];

        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
    }

    
    minX = std::max(0, minX - 1);
    maxX = std::min(9, maxX + 1);
    minY = std::max(0, minY - 1);
    maxY = std::min(9, maxY + 1);

    
    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            
            bool isShipCell = false;
            for (int i = 0; i < 5; ++i) {
                if (shipPositions[shipIndex][i][0] == x && 
                    shipPositions[shipIndex][i][1] == y) {
                    isShipCell = true;
                    break;
                }
            }

            
            if (!isShipCell && board[x][y] == '.') {
                board[x][y] = 'O';
                shotHistory[x][y] = true;
            }
        }
    }
}






