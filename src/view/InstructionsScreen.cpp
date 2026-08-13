#include "view/InstructionsScreen.h"
#include "utils/UiTheme.h"

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct InstructionLine {
    std::string text;
    bool heading;
};

const std::vector<InstructionLine> kLines = {
    {"OBJETIVO", true},
    {"Afunde todos os navios da frota inimiga antes que ela afunde a sua.", false},
    {"", false},
    {"POSICIONAMENTO", true},
    {"Clique no tabuleiro para posicionar o navio atual.", false},
    {"Botao direito do mouse gira o navio (horizontal / vertical).", false},
    {"Backspace ou R desfaz o ultimo navio posicionado.", false},
    {"", false},
    {"BATALHA", true},
    {"Clique numa celula do tabuleiro inimigo para atirar.", false},
    {"Quem acerta joga de novo; quem erra passa a vez para o oponente.", false},
    {"", false},
    {"TERRENO", true},
    {"Ilhas (cinza) bloqueiam qualquer navio.", false},
    {"Recifes (areia) so aceitam o navio Pequeno.", false},
    {"", false},
    {"VITORIA", true},
    {"Quando a frota de um lado for totalmente afundada, a partida termina", false},
    {"e sua pontuacao entra no ranking do mapa.", false},
};

}

InstructionsScreen::InstructionsScreen(sf::RenderWindow& window) : window(window) {
    if (!font.openFromFile("assets/fonts/Roboto-Regular.ttf"))
        throw std::runtime_error("Fonte nao encontrada: assets/fonts/Roboto-Regular.ttf");
}

void InstructionsScreen::show() {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) return;
            }
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) return;
            }
        }

        const float centerX = static_cast<float>(window.getSize().x) / 2.f;

        window.clear(ui::kSkyTop);
        ui::drawSeascape(window);
        ui::drawChartGrid(window);

        ui::drawCenteredText(window, font, "COMO JOGAR", centerX, 46.f, 34, ui::kGold, 1.6f);

        const float panelWidth = 640.f;
        const float panelHeight = 460.f;
        const float panelLeft = centerX - panelWidth / 2.f;
        const float panelTop = 118.f;

        sf::RectangleShape panel({panelWidth, panelHeight});
        panel.setPosition({panelLeft, panelTop});
        panel.setFillColor(ui::withAlpha(ui::kPanel, 226.f));
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(ui::withAlpha(ui::kInkSoft, 110.f));
        window.draw(panel);

        const float textLeft = panelLeft + 32.f;
        float y = panelTop + 28.f;

        for (const InstructionLine& line : kLines) {
            if (line.text.empty()) {
                y += 13.f;
                continue;
            }

            sf::Text text(font, line.text, line.heading ? 18u : 15u);
            text.setFillColor(line.heading ? ui::kGold : ui::kInk);
            if (line.heading) text.setLetterSpacing(1.3f);
            text.setPosition({textLeft, y});
            window.draw(text);

            y += line.heading ? 26.f : 22.f;
        }

        ui::drawCenteredText(window, font, "ESC ou clique para voltar", centerX,
                             panelTop + panelHeight + 20.f, 13, ui::withAlpha(ui::kInkSoft, 190.f), 1.6f);
        ui::drawHudFrame(window);
        window.display();
    }
}
