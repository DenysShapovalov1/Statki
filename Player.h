#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include <string>

class Player{
protected:
    Board board;
private:
   int wins;
public:

    std::string nickname;
    Player(const std::string& name);
    void displayBoards(const Player& opponent) const;
    void displayBoard() const;
    virtual void placeShips(); 
    virtual  bool shoot(Player& opponent);
    bool allShipsSunk() const;
    std::string getNickname() const;
    int getWins() const;
    void incrementWins();
    Board& getBoard() { return board; }
    void clearConsole();
    static void playTwoPlayers(int& player1Wins, int& player2Wins);
    static std::string getPlayerNickname(int playerNumber);
};



#endif 
