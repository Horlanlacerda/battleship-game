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
        sf::RenderWindow window(sf::VideoMode({800, 600}), "Batalha Naval - Posicionamento Manual");
        window.setFramerateLimit(60);

        std::cout << "Criando Tabuleiro 10x10..." << std::endl;
        Board board(10, 10);

        std::cout << "Criando Frota de Navios..." << std::endl;
        std::vector<Ship> ships;
        
        // Passando os 4 parametros exigidos pelo construtor do Ship
        ships.push_back(Ship(ShipType::SMALL, 0, 0, true));
        ships.push_back(Ship(ShipType::MEDIUM, 0, 0, true));
        ships.push_back(Ship(ShipType::LARGE, 0, 0, true));

        std::cout << "Iniciando PlacementScreen..." << std::endl;
        PlacementScreen placementScreen(window, board, ships);
        placementScreen.run();

        std::cout << "Posicionamento concluido com sucesso!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERRO: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}