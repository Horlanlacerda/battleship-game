#include "MenuScreen.h"
#include "PlacementScreen.h"
#include "RankingScreen.h"
#include "Database.h"
#include "GameState.h"
#include <iostream>

std::string mapTypeToString(MapType map) {
    switch (map) {
        case MapType::ACUDE:  return "ACUDE";
        case MapType::LAGO:   return "LAGO";
        case MapType::OCEANO: return "OCEANO";
    }
    return "OCEANO";
}

int main() {
    try {
        Database db("ranking.db");

        sf::RenderWindow window(sf::VideoMode({800, 600}), "Teste Batalha Naval");
        
        MenuScreen menu(window);
        RankingScreen ranking(window, db);

        while (window.isOpen()) {
            MenuOption choice = menu.showMainMenu();

            if (choice == MenuOption::START) {
                MapType map = menu.showMapSelection();
                std::string mapName = mapTypeToString(map);

                // Monta o tabuleiro e a frota corretos para o mapa escolhido
                // e deixa o jogador posicionar os navios antes da partida.
                GameState gameState(map);
                PlacementScreen placement(window, gameState.getPlayerBoard(), gameState.getPlayerShips());
                placement.run();

                if (!window.isOpen()) continue; // jogador fechou a janela durante o posicionamento

                int fakeScore = 1500;
                int fakeTime = 90;
                bool playerWon = true;

                std::string playerName = ranking.showGameOver(fakeScore, fakeTime, playerWon, mapName);

                db.saveResult({playerName, fakeScore, fakeTime, mapName});

                ranking.showRanking(mapName);
            }
            else if (choice == MenuOption::RANKING) {
                MapType map = menu.showMapSelection();
                ranking.showRanking(mapTypeToString(map));
            }
            else if (choice == MenuOption::EXIT) {
                window.close();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERRO]: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}