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
                std::cout << "Nie można umieścić statek. Spróbuj ponownie.\n";
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
    board.hideShips();
}


bool Player::shoot(Player& opponent) {
    int x, y;
    while (true) {
        std::cout << nickname << ", wprowadź współrzędne do strzału (x y): ";
        std::cin >> x >> y;

        if (std::cin.fail() || x < 0 || x > 9 || y < 0 || y > 9) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Błąd: wybierz liczby od 0 do 9.\n";
            continue; 
        }

        
        if (opponent.board.shoot(x, y)) {
            std::cout << "Trafiłeś!\n";
            displayBoards(opponent);
            if (opponent.allShipsSunk()) {
                std::cout << opponent.nickname << " wszystkie statki zostały zatopione!\n";
                return false; 
            }
        } else {
            std::cout << "Nie trafiłeś.\n";
            clearConsole();
            break; 
        }

        clearConsole();
        displayBoards(opponent);
        
    }
    return true; 
}

bool Player::allShipsSunk() const {
    return board.allShipsSunk();
}

std::string Player::getNickname() const {
    return nickname;
}




void Player::playTwoPlayers(int& player1Wins, int& player2Wins) {
    std::string player1Name, player2Name;
    std::cout << "Podaj imię gracza 1: ";
    std::cin >> player1Name;

    while (true) {
        std::cout << "Podaj imię gracza 2: ";
        std::cin >> player2Name;

        if (player1Name == player2Name) {
            std::cout << "Nie możesz używać takiego samego imienia, wybierz coś innego! \n";
        } else {
            break;
        }
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