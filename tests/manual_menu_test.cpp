#include "view/MenuScreen.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Teste MenuScreen");
    MenuScreen menu(window);

    MenuOption choice = menu.showMainMenu();

    switch (choice) {
        case MenuOption::START:        std::cout << "Iniciar Jogo\n"; break;
        case MenuOption::RANKING:      std::cout << "Ranking\n"; break;
        case MenuOption::INSTRUCTIONS: std::cout << "Instrucoes\n"; break;
        case MenuOption::EXIT:         std::cout << "Sair\n"; break;
    }

    return 0;
}