<<<<<<< HEAD
#include <SFML/Graphics.hpp>

int main(){

    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "BattleShip Game"
    );

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(50, 80, 150));
        window.display();   
    }
    
    return 0;
}

=======
#include <iostream>
#include "Ship.h"

int main() {
    std::cout << "Starting BattleShipGame..." << std::endl;
    Ship ship(ShipType::SMALL, 0, 0, true);
    std::cout << "Created ship of type: " << ship.getTypeName() << std::endl;
    return 0;
}
>>>>>>> dev
