#include <iostream>
#include "Ship.h"

int main() {
    std::cout << "Starting BattleShipGame..." << std::endl;
    Ship ship(ShipType::SMALL, 0, 0, true);
    std::cout << "Created ship of type: " << ship.getTypeName() << std::endl;
    return 0;
}