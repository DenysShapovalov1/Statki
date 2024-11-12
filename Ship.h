#ifndef SHIP_H
#define SHIP_H

#include <string>



class Ship {
public:
    int x, y; 
    int size; 
    std::string name;
    bool horizontal; 

    Ship(int x, int y, int size, const std::string& name, bool horizontal);
};

#endif 