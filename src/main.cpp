#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <optional>

#include "model/Board.h"
#include "model/Ship.h"
#include "view/PlacementScreen.h"

int main() {
    try {
        std::cout << "Criando janela SFML..." << std::endl;
        sf::RenderWindow window(
            sf::VideoMode({800, 600}),
            "Batalha Naval - Posicionamento Manual"
        );
        window.setFramerateLimit(60);

        std::cout << "Criando Tabuleiro 10x10..." << std::endl;
        Board board(10, 10);

        std::cout << "Criando Frota de Navios..." << std::endl;
        std::vector<Ship> ships;
        ships.push_back(Ship(ShipType::SMALL, 0, 0, true));
        ships.push_back(Ship(ShipType::MEDIUM, 0, 0, true));
        ships.push_back(Ship(ShipType::LARGE, 0, 0, true));

        // ==========================================
        // TELA DE POSICIONAMENTO MANUAL
        // ==========================================
        std::cout << "Iniciando PlacementScreen..." << std::endl;
        std::cout << "-> Clique Esquerdo: Colocar Navio" << std::endl;
        std::cout << "-> Clique Direito: Girar Navio" << std::endl;
        std::cout << "-> Tecla Backspace ou R: Desfazer / Reposicionar Navio Anterior" << std::endl;

        PlacementScreen placementScreen(window, board, ships);
        placementScreen.run();

        std::cout << "Posicionamento concluido com sucesso!" << std::endl;

        // Loop final para visualizar o resultado
        BoardRenderer finalRenderer(window, 40.f, 50.f, 50.f);
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
            }
            window.clear(sf::Color(15, 30, 60));
            finalRenderer.draw(board, false);
            window.display();
        }

    } catch (const std::exception& e) {
        std::cerr << "ERRO: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}