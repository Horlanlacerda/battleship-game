#include "RankingScreen.h"
#include "utils/UiTheme.h"
#include <algorithm>
#include <string>
#include <optional>

RankingScreen::RankingScreen(sf::RenderWindow& window, Database& db)
    : window(window), db(db)
{
    font.openFromFile("assets/fonts/Roboto-Regular.ttf");
}

void RankingScreen::showRanking(const std::string& mapType) {
    auto entries = db.getTopScores(mapType, 10);

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

        const float centerX = static_cast<float>(window.getSize().x) / 2.f;

        window.clear(ui::kSkyTop);
        ui::drawSeascape(window);
        ui::drawChartGrid(window);

        ui::drawCenteredText(window, font, "RANKING - " + mapType, centerX, 46.f, 34, ui::kGold, 1.6f);

        // Painel com a lista de pontuacoes, mesmo estilo dos cartoes do menu.
        const float panelWidth = 480.f;
        const float panelLeft = centerX - panelWidth / 2.f;
        const float panelTop = 118.f;
        const float rowHeight = 34.f;
        const std::size_t rowCount = std::max<std::size_t>(entries.size(), 1);
        const float panelHeight = 56.f + rowHeight * rowCount;

        sf::RectangleShape panel({panelWidth, panelHeight});
        panel.setPosition({panelLeft, panelTop});
        panel.setFillColor(ui::withAlpha(ui::kPanel, 226.f));
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(ui::withAlpha(ui::kInkSoft, 110.f));
        window.draw(panel);

        sf::Text header(font, "#   Nome               Pontos    Tempo", 16);
        header.setFillColor(ui::kInkSoft);
        header.setPosition({panelLeft + 20.f, panelTop + 16.f});
        window.draw(header);

        if (entries.empty()) {
            sf::Text empty(font, "Nenhum resultado registrado ainda", 15);
            empty.setFillColor(ui::kInkSoft);
            empty.setPosition({panelLeft + 20.f, panelTop + 52.f});
            window.draw(empty);
        }

        for (std::size_t i = 0; i < entries.size(); ++i) {
            auto& e = entries[i];

            std::string padding(std::max(0, 18 - (int)e.playerName.size()), ' ');
            std::string line = std::to_string(i + 1) + ". " + e.playerName + padding +
                               std::to_string(e.score) + "       " + std::to_string(e.elapsedSeconds) + "s";

            sf::Text row(font, line, 18);
            row.setFillColor(i == 0 ? ui::kGold : ui::kInk); // Ouro para o 1º
            row.setPosition({panelLeft + 20.f, panelTop + 50.f + i * rowHeight});
            window.draw(row);
        }

        ui::drawCenteredText(window, font, "ESC para voltar", centerX, 556.f, 13, ui::withAlpha(ui::kInkSoft, 190.f), 1.6f);
        ui::drawHudFrame(window);
        window.display();
    }
}

std::string RankingScreen::showGameOver(int score, int elapsedSec, bool playerWon, const std::string& mapType) {
    std::string playerName = "";
    bool typing = true;

    const std::string resultText = playerWon ? "VOCE VENCEU!" : "VOCE PERDEU!";
    const sf::Color resultColor = playerWon ? sf::Color(90, 230, 150) : sf::Color(230, 90, 90);

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

        const float centerX = static_cast<float>(window.getSize().x) / 2.f;

        window.clear(ui::kSkyTop);
        ui::drawSeascape(window);
        ui::drawChartGrid(window);

        ui::drawCenteredText(window, font, resultText, centerX, 130.f, 44, resultColor, 1.8f, 3.f, sf::Color(8, 20, 40));
        ui::drawCenteredText(window, font, "Mapa: " + mapType, centerX, 200.f, 16, ui::kInkSoft, 1.4f);
        ui::drawCenteredText(window, font,
                             "Pontuacao: " + std::to_string(score) + "   |   Tempo: " + std::to_string(elapsedSec) + "s",
                             centerX, 232.f, 22, ui::kInk, 1.2f);

        ui::drawCenteredText(window, font, "Digite seu nome e pressione ENTER:", centerX, 300.f, 18, ui::kInkSoft, 1.4f);

        // Campo de nome, com o mesmo tom de painel usado nas outras telas.
        const float fieldWidth = 320.f;
        sf::RectangleShape field({fieldWidth, 44.f});
        field.setPosition({centerX - fieldWidth / 2.f, 338.f});
        field.setFillColor(ui::withAlpha(ui::kPanel, 226.f));
        field.setOutlineThickness(2.f);
        field.setOutlineColor(ui::kGold);
        window.draw(field);

        ui::drawCenteredText(window, font, playerName + "|", centerX, 348.f, 22, sf::Color(140, 210, 255), 1.f); //'|' piscando para saber onde digitar

        ui::drawHudFrame(window);
        window.display();
    }

    return playerName.empty() ? "Anonimo" : playerName; //se não digitar por algum acaso
}