#include <SFML/Graphics.hpp>
#include <iostream>
#include "model/Board.h"
#include "view/BoardRenderer.h"
#include "controller/GameController.h"

int main() {
    Board board(5, 5);
    // Posiciona um navio de tamanho 3 para podermos testar os acertos
    board.placeShip(0, 0, 3, true);

    sf::RenderWindow window(sf::VideoMode({600, 600}), "Teste GameController - Armas Especiais");
    BoardRenderer renderer(window, 60.f, 80.f, 80.f);
    GameController controller(60.f, 80.f, 80.f, 5, 5);

    std::cout << "\n==========================================" << std::endl;
    std::cout << "      CONTROLES DO TESTE DE ARMAS        " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Pressione [1] -> Selecionar Tiro Basico" << std::endl;
    std::cout << "Pressione [2] -> Selecionar Missil (Area 2x2)" << std::endl;
    std::cout << "Pressione [3] -> Selecionar Torpedo (Linha Inteira)" << std::endl;
    std::cout << "==========================================\n" << std::endl;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // 1. Troca de Arma via Teclas (1, 2, 3)
            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Num1) {
                    controller.selectWeapon(WeaponType::BASIC);
                    std::cout << "-> Arma Ativa: TIRO BASICO" << std::endl;
                }
                else if (keyPress->code == sf::Keyboard::Key::Num2) {
                    controller.selectWeapon(WeaponType::MISSILE);
                    std::cout << "-> Arma Ativa: MISSIL (Restantes: " 
                              << controller.getMissilesLeft() << ")" << std::endl;
                }
                else if (keyPress->code == sf::Keyboard::Key::Num3) {
                    controller.selectWeapon(WeaponType::TORPEDO);
                    std::cout << "-> Arma Ativa: TORPEDO (Restantes: " 
                              << controller.getTorpedoesLeft() << ")" << std::endl;
                }
            }

            // 2. Disparo via Clique do Mouse
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    int row, col;
                    sf::Vector2i mousePos(mouseButton->position.x, mouseButton->position.y);

                    if (controller.screenToBoardCoord(mousePos, row, col)) {
                        WeaponType currentWeapon = controller.getSelectedWeapon();

                        if (currentWeapon == WeaponType::BASIC) {
                            CellState result = controller.fireBasic(board, row, col);
                            std::cout << "Tiro Basico em (" << row << ", " << col << ") -> ";
                            std::cout << (result == CellState::HIT ? "ACERTOU!" : "AGUA!") << std::endl;
                        } 
                        else if (currentWeapon == WeaponType::MISSILE) {
                            int hits = controller.fireMissile(board, row, col);
                            std::cout << "MISSIL em (" << row << ", " << col << ") -> " 
                                      << hits << " acerto(s)! Misseis restantes: " 
                                      << controller.getMissilesLeft() << std::endl;
                        } 
                        else if (currentWeapon == WeaponType::TORPEDO) {
                            int hits = controller.fireTorpedo(board, row, col);
                            std::cout << "TORPEDO na Linha " << row << " -> " 
                                      << hits << " acerto(s)! Torpedos restantes: " 
                                      << controller.getTorpedoesLeft() << std::endl;
                        }
                        // Verificação de vitória após qualquer disparo
                        if (!board.hasAnyShipRemaining()) {
                            std::cout << "\n FIM DE JOGO! TODOS OS NAVIOS FORAM AFUNDADOS! \n" << std::endl;
                        }

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
