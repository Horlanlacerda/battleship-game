#include "view/MenuScreen.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Teste MenuScreen");
    MenuScreen menu(window);

    MenuOption choice = menu.showMainMenu();
    if (choice == MenuOption::START) {
        MapType map = menu.showMapSelection();
        switch (map) {
            case MapType::POCA:    std::cout << "Poca\n"; break;
            case MapType::LAGO:    std::cout << "Lago\n"; break;
            case MapType::OCEANO:  std::cout << "Oceano\n"; break;
        }
    }

    return 0;
}