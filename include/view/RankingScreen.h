#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Database.h" 

class RankingScreen {
public:
    RankingScreen(sf::RenderWindow& window, Database& db);

    void showRanking(const std::string& mapType);

    std::string showGameOver(int score, int elapsedSec, bool playerWon, const std::string& mapType);

private:
    sf::RenderWindow& window;
    Database& db;
    sf::Font font;
};