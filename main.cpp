#include "Player.cc"
#include "Board.cc"
#include "Ship.cc"
#include <iostream>
#include <limits>


using namespace std;

void displayMenu() {
    cout << "=== Menu ===\n";
    cout << "1. Gra 1 na 1\n";
    cout << "2. Wyjście\n";
    cout << "Wybierz opcje: ";
}


int main() {
    int player1Wins = 0;
    int player2Wins = 0;
    int playerWins = 0;
    int computerWins = 0;
    int playAgain;

    system("chcp 65001");
    system("cls");


    while (true) {
        displayMenu();
        int choice; 
        while (true) {
            cin >> choice;
            if (cin.fail()) { 
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Niewłaściwy wybór. Proszę spróbować ponownie: ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                break; 
            }
        }

        if (choice == 1) {
            Player::playTwoPlayers(player1Wins, player2Wins);
            system("cls");
            while (true) {
                cout << "Czy chcesz zagrać ponownie? (1 - Tak, 0 - Nie): ";
                cin >> playAgain;
            if (cin.fail() || (playAgain != 1 && playAgain != 0)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Błąd: wybierz Tak (1) lub Nie (0).\n";
            } else {
            if (playAgain == 0) {
                break;
                }
            }
        }
            } else if (choice == 2) {
                cout << "Dziękujemy za grę!\n";
                return 0;
            } else {
                cout << "Niewłaściwy wybór. Proszę spróbować ponownie.\n";
            }

        }
}
