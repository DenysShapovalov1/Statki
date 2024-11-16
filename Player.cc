#include "Player.h"
#include <iostream>
#include <limits>
#include <cstdlib>

Player::Player(const std::string& name) : nickname(name), wins(0) {}

void Player::displayBoard() const {
  std::cout << "Plansza gracza " << nickname << ":\n";
   board.displayBoard(); 
}

void Player::displayBoards(const Player& opponent) const {
    std::cout << "Plansza gracza " << nickname << ":\n";
    board.displayBoards(opponent.board); 
}

int Player::getWins() const {
        return wins;
    }

    
void Player::incrementWins() {
        wins++;
    }


void Player::clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Player::placeShips() {
    int sizes[4] = {1, 2, 3, 4}; 
    int counts[4] = {4, 3, 2, 1}; 
    std::string names[4] = {"Jednopokładowy", "Dwupokładowy", "Trzypokładowy", "Czteropokładowy"};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < counts[i]; ++j) {
            int x, y;
            bool horizontal;
            std::cout << "Gracz " << nickname << ", umieść " << names[i] << " (rozmiar " << sizes[i] << "):\n";

            while (true) {
                std::cout << "Podaj współrzędne (x y): ";
                std::cin >> x >> y;

                
                if (std::cin.fail() || x < 0 || x > 9 || y < 0 || y > 9) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    std::cout << "Błąd: wybierz liczby od 0 do 9.\n";
                } else {
                    break; 
                }
            }

            if (sizes[i] > 1) {
                std::cout << "Czy statek jest poziomy? (1 - tak, 0 - nie): ";
                std::cin >> horizontal;

                
                while (std::cin.fail() || (horizontal != 0 && horizontal != 1)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Błąd: wybierz 1 (tak) lub 0 (nie).\n";
                    std::cout << "Czy statek jest poziomy? (1 - tak, 0 - nie): ";
                    std::cin >> horizontal;
                }
            }

            while (!board.placeShip(x, y, sizes[i], names[i], horizontal)) {
                while (true) {
                    std::cout << "Podaj współrzędne (x y): ";
                    std::cin >> x >> y;

                    
                    if (std::cin.fail() || x < 0 || x > 9 || y < 0 || y > 9) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Błąd: wybierz liczby od 0 do 9.\n";
                    } else {
                        break; 
                    }
                }

                if (sizes[i] > 1) {
                    std::cout << "Czy statek jest poziomy? (1 - tak, 0 - nie): ";
                    std::cin >> horizontal;

                    
                    while (std::cin.fail() || (horizontal != 0 && horizontal != 1)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Błąd: wybierz 1 (tak) lub 0 (nie).\n";
                        std::cout << "Czy statek jest poziomy? (1 - tak, 0 - nie): ";
                        std::cin >> horizontal;
                    }
                }
            }
            
            displayBoard();
            
        }
    }
    clearConsole();
}

bool Player::shoot(Player& opponent) {
    int x, y;
    while (true) {
        std::cout << nickname << ", podaj współrzędne do strzału (x y): ";
        std::cin >> x >> y;

        if (std::cin.fail() || x < 0 || x > 9 || y < 0 || y > 9) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Błąd: wybierz liczby od 0 do 9.\n";
            continue; 
        }

        if (!opponent.board.shotHistory[x][y]) { 
            if (opponent.board.shoot(x, y)) {
                std::cout << "Trafiłeś!\n";
                if (opponent.allShipsSunk()) {
                std::cout << opponent.nickname << " wszystkie statki zostały zatopione!\n";
                return false; 
            }
            } else {
                std::cout << "Nie Trafiłęś!\n";
                break; 
            }
        } else {
            std::cout << "Już strzelałeś w to miejsce! Spróbuj jeszcze raz.\n";
            continue; 
        }

        clearConsole();
        displayBoards(opponent);
    }
    clearConsole();
    std::cout << "Czy następny gracz jest gotowy? Wpisz dowolny klawisz, aby kontynuować...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    return true; 
}

bool Player::allShipsSunk() const {
    return board.allShipsSunk();
}

std::string Player::getNickname() const {
    return nickname;
}

void Player::playTwoPlayers(int& player1Wins, int& player2Wins) {
    std::string player1Name = getPlayerNickname(1);
    std::string player2Name = getPlayerNickname(2);

    
    while (player1Name == player2Name) {
        std::cout << "Imiona graczy nie mogą być takie same! Wybierz inne imię dla gracza 2.\n";
        player2Name = getPlayerNickname(2); 
    }

    Player player1(player1Name);
    Player player2(player2Name);

    player1.placeShips();
    player2.placeShips();

    time_t startTime = time(0); 
    bool gameActive = true;
    while (gameActive) {
        player1.displayBoards(player2); 
        if (!player1.shoot(player2)) {
            gameActive = false; 
            break;
        }

        player2.displayBoards(player1);
        if (!player2.shoot(player1)) {
            gameActive = false; 
            break;
        }
    }

    time_t endTime = time(0); 
    double duration = difftime(endTime, startTime); 
    if (player1.allShipsSunk()) {
        std::cout << player2.getNickname() << " wygrał!\n";
        player2Wins++; 
    } else {
        std::cout << player1.getNickname() << " wygrał!\n";
        player1Wins++; 
    }
    std::cout << "Czas trwania gry: " << duration << " sekund.\n";
    std::cout << "Wygrane - Gracz 1: " << player1Wins << ", Gracz 2: " << player2Wins << "\n";
}

std::string Player::getPlayerNickname(int playerNumber) {
    std::string playerName;
    
    
    std::cout << "Podaj imię gracza " << playerNumber << ": ";
    std::cin >> playerName;

    return playerName;
}

