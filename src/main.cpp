#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "model/Board.h"
#include "model/Ship.h"
#include "view/PlacementScreen.h"
#include "controller/AutoPlacer.h"

int main() {
    try {
        std::cout << "Criando janela SFML..." << std::endl;
        sf::RenderWindow window(
            sf::VideoMode({800, 600}),
            "Batalha Naval - Posicionamento"
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
        // TESTE DO AUTOPLACER
        // ==========================================

        std::cout << "Testando posicionamento automatico..." << std::endl;

        AutoPlacer autoPlacer;

        if (autoPlacer.place(board, ships)) {
            std::cout << "Navios posicionados automaticamente!" << std::endl;

            for (const Ship& ship : ships) {
                std::cout << "Navio: "
                          << "linha = " << ship.getStartRow()
                          << ", coluna = " << ship.getStartCol()
                          << std::endl;
            }
        } else {
            std::cout << "Falha no posicionamento automatico!" << std::endl;
        }

        // ==========================================
        // TESTE DO PLACEMENTSCREEN
        // ==========================================

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