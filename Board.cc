#include "Board.h"

Board::Board() : shipCount(0) {
    resetShotHistory();
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            board[i][j] = '.';
            shotHistory[i][j] = false;
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

bool Board::canPlaceShip(int x, int y, int size, bool horizontal, std::string& errorMessage) {
    if (horizontal) {
        if (y + size > 10) {
            errorMessage = "Statek wykracza poza granice planszy w poziomie.";
            return false;
        }
    } else {
        if (x + size > 10) {
            errorMessage = "Statek wykracza poza granice planszy w pionie.";
            return false;
        }
    }
    for (int i = 0; i < size; ++i) {
        int newX = horizontal ? x : x + i;
        int newY = horizontal ? y + i : y;
        if (newX < 0 || newX >= 10 || newY < 0 || newY >= 10 || board[newX][newY] != '.') {
            errorMessage = "Statek koliduje z innym statkiem.";
            return false; 
        }
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = newX + dx;
                int ny = newY + dy;
                if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && board[nx][ny] != '.') {
                    errorMessage = "Statek nie może być umieszczony zbyt blisko innego statku.";
                    return false;
                }
            }
        }
    }

    return true; 
}

bool Board::placeShip(int x, int y, int size, const std::string& name, bool horizontal) { 
    std::string errorMessage;
    if (!canPlaceShip(x, y, size, horizontal, errorMessage)) {
        std::cout << "Nie można umieścić statku: " << errorMessage << "\n";
        return false; 
    }

    ships.push_back(Ship(x, y, size, name, horizontal));
    for (int i = 0; i < size; ++i) {
        if (horizontal) {
            board[x][y + i] = name[0]; 
        } else {
            board[x + i][y] = name[0]; 
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
        if (isShipSunk(x, y) && !allShipsSunk()) {
            std::cout << "Statek zniszczony!\n";
            if (allShipsSunk()) {
                std::cout << "Wszystkie statki zatopione!\n";
                return true; 
            }
        }
        return true; 
    }

    return false; 
}

bool Board::isShipSunk(int x, int y) {
    int shipIndex = findShipIndex(x, y);
    if (shipIndex == -1) return false; 
    
    for (int i = 0; i < ships[shipIndex].size; ++i) {
        int partX = ships[shipIndex].horizontal ? ships[shipIndex].x : ships[shipIndex].x + i;
        int partY = ships[shipIndex].horizontal ? ships[shipIndex].y + i : ships[shipIndex].y;

        if (board[partX][partY] != 'X') {
            return false; 
        }
    }

    std::cout << "Statek " << ships[shipIndex].name << " (" << ships[shipIndex].size << "-pokładowy) został zatopiony!" << std::endl; 
    markSurroundingCellsAsMissed(shipIndex); 
    return true; 
}

int Board::findShipIndex(int x, int y) {
    for (int i = 0; i < shipCount; ++i) {
        if (ships[i].horizontal) {
            if (x == ships[i].x && y >= ships[i].y && y < ships[i].y + ships[i].size) {
                return i;
            }
        } else {
            if (y == ships[i].y && x >= ships[i].x && x < ships[i].x + ships[i].size) {
                return i;
            }
        }
    }
    return -1; 
}

bool Board::allShipsSunk() const {
    for (int i = 0; i < shipCount; ++i) {
        bool shipSunk = true;
        for (int j = 0; j < ships[i].size; ++j) {
            int partX = ships[i].horizontal ? ships[i].x : ships[i].x + j;
            int partY = ships[i].horizontal ? ships[i].y + j : ships[i].y;

            
            if (board[partX][partY] != 'X') {
                shipSunk = false;
                break;
            }
        }
        if (!shipSunk) {
            return false;
        }
    }
    return true; 
}
void Board::markSurroundingCellsAsMissed(int shipIndex) {
    int minX = ships[shipIndex].x - 1;
    int maxX = ships[shipIndex].horizontal ? ships[shipIndex].x + 1 : ships[shipIndex].x + ships[shipIndex].size;
    int minY = ships[shipIndex].y - 1;
    int maxY = ships[shipIndex].horizontal ? ships[ shipIndex].y + ships[shipIndex].size : ships[shipIndex].y + 1;

    for (int x = std::max(0, minX); x <= std::min(9, maxX); ++x) {
        for (int y = std::max(0, minY); y <= std::min(9, maxY); ++y) {
            if (board[x][y] == '.') {
                board[x][y] = 'O';
                shotHistory[x][y] = true;
            }
        }
    }
}
