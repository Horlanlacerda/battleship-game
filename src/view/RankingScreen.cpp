#include "RankingScreen.h"
#include <string>
#include <optional>

RankingScreen::RankingScreen(sf::RenderWindow& window, Database& db)
    : window(window), db(db) 
{
    font.openFromFile("assets/fonts/Roboto-Regular.ttf");
}

void RankingScreen::showRanking(const std::string& mapType) {
    auto entries = db.getTopScores(mapType, 10);

    sf::Text title(font, "RANKING - " + mapType, 32);
    title.setFillColor(sf::Color::White);
    title.setPosition({200.f, 40.f});

    sf::Text back(font, "ESC para voltar", 18);
    back.setFillColor(sf::Color(150, 150, 150));
    back.setPosition({20.f, 560.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    return;
                }
            }
        }

        window.clear(sf::Color(15, 30, 60));
        window.draw(title);
        window.draw(back);

        sf::Text header(font, "# Nome               Pontos    Tempo", 18);
        header.setFillColor(sf::Color(180, 180, 180));
        header.setPosition({60.f, 100.f});
        window.draw(header);

        for (int i = 0; i < (int)entries.size(); i++) {
            auto& e = entries[i];
            
            std::string padding(std::max(0, 18 - (int)e.playerName.size()), ' ');
            std::string line = std::to_string(i + 1) + ". " + e.playerName + padding + 
                               std::to_string(e.score) + "       " + std::to_string(e.elapsedSeconds) + "s";
            
            sf::Text row(font, line, 20);
            row.setFillColor(i == 0 ? sf::Color(255, 215, 0) : sf::Color::White); // Ouro para o 1º
            row.setPosition({60.f, 130.f + i * 36.f});
            window.draw(row);
        }

        window.display();
    }
}

std::string RankingScreen::showGameOver(int score, int elapsedSec, bool playerWon, const std::string& mapType) {
    std::string playerName = "";
    bool typing = true;

    sf::Text result(font, playerWon ? "VOCE VENCEU!" : "VOCE PERDEU!", 40);
    result.setFillColor(playerWon ? sf::Color(0, 220, 100) : sf::Color(220, 60, 60));
    result.setPosition({200.f, 80.f});

    sf::Text scoreText(font, "Pontuacao: " + std::to_string(score) + " | Tempo: " + std::to_string(elapsedSec) + "s", 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({150.f, 160.f});

    sf::Text prompt(font, "Digite seu nome e pressione ENTER:", 20);
    prompt.setFillColor(sf::Color(200, 200, 200));
    prompt.setPosition({150.f, 240.f});

    while (window.isOpen() && typing) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return playerName.empty() ? "Anonimo" : playerName;
            }
            
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                char c = static_cast<char>(textEntered->unicode);
                if (c == '\r' || c == '\n') {
                    typing = false;
                } else if (c == '\b') {
                    if (!playerName.empty()) playerName.pop_back();
                } else if (playerName.size() < 16 && c >= 32) {
                    playerName += c;
                }
            }
        }

        sf::Text nameField(font, playerName + "|", 26); //'|' piscando para saber onde digitar
        nameField.setFillColor(sf::Color(100, 200, 255));
        nameField.setPosition({150.f, 280.f});

        window.clear(sf::Color(15, 30, 60));
        window.draw(result);
        window.draw(scoreText);
        window.draw(prompt);
        window.draw(nameField);
        window.display();
    }

    return playerName.empty() ? "Anonimo" : playerName; //se não digitar por algum acaso
}