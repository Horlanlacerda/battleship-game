#include "MenuScreen.h"
#include "PlacementScreen.h"
#include "GameScreen.h"
#include "RankingScreen.h"
#include "InstructionsScreen.h"
#include "Database.h"
#include "GameState.h"
#include "ScoreCalculator.h"
#include "UiTheme.h"
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

        sf::RenderWindow window(sf::VideoMode({ui::kWindowWidth, ui::kWindowHeight}), "Teste Batalha Naval",
                                sf::Style::Titlebar | sf::Style::Close);

        MenuScreen menu(window);
        RankingScreen ranking(window, db);
        InstructionsScreen instructions(window);

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

                // Batalha: jogador vs computador (a IA posiciona sua propria
                // frota automaticamente ao construir a GameScreen).
                GameScreen gameScreen(window, gameState);
                GameScreen::Outcome outcome = gameScreen.run();

                if (!window.isOpen()) continue; // jogador fechou a janela durante a partida

                const bool playerWon = (outcome.result == GameResult::PLAYER_GANHOU);
                const GameData gameData{outcome.hits, outcome.misses, outcome.shipsDestroyed,
                                        outcome.alliedShipsSurvived, outcome.elapsedSeconds};
                const int score = ScoreCalculator::calculate(gameData);

                std::string playerName = ranking.showGameOver(score, outcome.elapsedSeconds, playerWon, mapName);

                db.saveResult({playerName, score, outcome.elapsedSeconds, mapName});

                ranking.showRanking(mapName);
            }
            else if (choice == MenuOption::RANKING) {
                MapType map = menu.showMapSelection();
                ranking.showRanking(mapTypeToString(map));
            }
            else if (choice == MenuOption::INSTRUCTIONS) {
                instructions.show();
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