#include "Ship.h"

Ship::Ship(int x, int y, int size, const std::string& name, bool horizontal)
    : x(x), y(y), size(size), name(name), horizontal(horizontal) {}