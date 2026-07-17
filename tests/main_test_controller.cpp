#include <SFML/Graphics.hpp>
#include <iostream>
#include "model/Board.h"
#include "view/BoardRenderer.h"
#include "controller/GameController.h"

int main() {
    Board board(5, 5);
    board.placeShip(0, 0, 3, true);

    sf::RenderWindow window(sf::VideoMode({600, 600}), "Teste GameController");
    BoardRenderer renderer(window, 60.f, 80.f, 80.f);
    GameController controller(60.f, 80.f, 80.f, 5, 5);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    int row, col;
                    sf::Vector2i mousePos(mouseButton->position.x, mouseButton->position.y);

                    if (controller.screenToBoardCoord(mousePos, row, col)) {
                        std::cout << "Clique valido em: (" << row << ", " << col << ")" << std::endl;
                        board.shoot(row, col); // testa integrando com o Board
                    } else {
                        std::cout << "Clique fora do tabuleiro" << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color(15, 30, 60));
        renderer.draw(board, false);
        window.display();
    }

    return 0;
}