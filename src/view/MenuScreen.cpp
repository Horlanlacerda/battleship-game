#include "view/MenuScreen.h"
#include <MapLayout.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

const sf::Color kSkyTop   (  6,  12,  30);
const sf::Color kPanel    ( 16,  44,  74);
const sf::Color kPanelHot ( 32,  86, 132);
const sf::Color kGold     (238, 190,  90);
const sf::Color kInk      (214, 232, 244);
const sf::Color kInkSoft  (132, 164, 190);
const sf::Color kShadow   (  0,   6,  16);

sf::Color withAlpha(sf::Color color, float alpha) {
    color.a = static_cast<std::uint8_t>(std::clamp(alpha, 0.f, 255.f));
    return color;
}

void drawLine(sf::RenderWindow& window, sf::Vector2f a, sf::Vector2f b, sf::Color color) {
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0] = sf::Vertex{a, color};
    line[1] = sf::Vertex{b, color};
    window.draw(line);
}

} // namespace

MenuScreen::MenuScreen(sf::RenderWindow& window) : window(window) {
    if (!font.openFromFile("assets/fonts/Roboto-Regular.ttf"))
        throw std::runtime_error("Fonte nao encontrada: assets/fonts/Roboto-Regular.ttf");
}

void MenuScreen::drawSeascape() {
    sf::CircleShape moonDisc(18.f);
    moonDisc.setOrigin({18.f, 18.f});
    moonDisc.setFillColor(sf::Color(226, 238, 250));
    window.draw(moonDisc);
}

void MenuScreen::drawChartGrid() {
    const float w = static_cast<float>(window.getSize().x);
    const float h = static_cast<float>(window.getSize().y);
    const sf::Color grid = withAlpha(sf::Color(140, 200, 230), 16.f);

    for (float x = 0.f; x <= w; x += 40.f)
        drawLine(window, {x, 0.f}, {x, h}, grid);
    for (float y = 0.f; y <= h; y += 40.f)
        drawLine(window, {0.f, y}, {w, y}, grid);
}

void MenuScreen::drawHudFrame() {
    const float w = static_cast<float>(window.getSize().x);
    const float h = static_cast<float>(window.getSize().y);

    sf::RectangleShape border({w - 24.f, h - 24.f});
    border.setPosition({12.f, 12.f});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(1.f);
    border.setOutlineColor(withAlpha(kGold, 55.f));
    window.draw(border);

    const float arm = 26.f;
    const float thick = 3.f;
    const sf::Vector2f corners[4] = {{12.f, 12.f}, {w - 12.f, 12.f}, {12.f, h - 12.f}, {w - 12.f, h - 12.f}};
    const sf::Vector2f dirs[4] = {{1.f, 1.f}, {-1.f, 1.f}, {1.f, -1.f}, {-1.f, -1.f}};

    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape horizontal({arm, thick});
        horizontal.setPosition({corners[i].x - (dirs[i].x < 0 ? arm : 0.f),
                                corners[i].y - (dirs[i].y < 0 ? thick : 0.f)});
        horizontal.setFillColor(withAlpha(kGold, 170.f));
        window.draw(horizontal);

        sf::RectangleShape vertical({thick, arm});
        vertical.setPosition({corners[i].x - (dirs[i].x < 0 ? thick : 0.f),
                              corners[i].y - (dirs[i].y < 0 ? arm : 0.f)});
        vertical.setFillColor(withAlpha(kGold, 170.f));
        window.draw(vertical);
    }
}

void MenuScreen::drawCenteredText(const std::string& text,
                                  float centerX,
                                  float y,
                                  unsigned size,
                                  sf::Color color,
                                  float letterSpacing,
                                  float outlineThickness,
                                  sf::Color outlineColor) {
    sf::Text label(font, text, size);
    label.setLetterSpacing(letterSpacing);
    label.setFillColor(color);
    label.setOutlineThickness(outlineThickness);
    label.setOutlineColor(outlineColor);

    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y});
    label.setPosition({centerX, y});
    window.draw(label);
}

int MenuScreen::pollClick(const std::vector<sf::FloatRect>& areas) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f click = {(float)mousePressed->position.x, (float)mousePressed->position.y};
                for (std::size_t i = 0; i < areas.size(); ++i)
                    if (areas[i].contains(click)) return static_cast<int>(i);
            }
        }
    }
    return -1;
}

void MenuScreen::drawPanel(sf::FloatRect area, bool hovered, float time) {
    if (hovered) {
        const float pulse = 0.5f + 0.5f * std::sin(time * 4.f);
        sf::RectangleShape glow({area.size.x + 14.f, area.size.y + 14.f});
        glow.setPosition({area.position.x - 7.f, area.position.y - 7.f});
        glow.setFillColor(sf::Color::Transparent);
        glow.setOutlineThickness(1.f);
        glow.setOutlineColor(withAlpha(kGold, 60.f + 70.f * pulse));
        window.draw(glow);
    }

    sf::RectangleShape shade({area.size.x, area.size.y});
    shade.setPosition({area.position.x + 4.f, area.position.y + 6.f});
    shade.setFillColor(withAlpha(kShadow, 120.f));
    window.draw(shade);

    sf::RectangleShape plate({area.size.x, area.size.y});
    plate.setPosition(area.position);
    plate.setFillColor(withAlpha(hovered ? kPanelHot : kPanel, 226.f));
    plate.setOutlineThickness(2.f);
    plate.setOutlineColor(hovered ? kGold : withAlpha(kInkSoft, 110.f));
    window.draw(plate);
}

void MenuScreen::drawTitle() {
    drawCenteredText("BATALHA NAVAL", static_cast<float>(window.getSize().x) / 2.f, 62.f, 56,
                     kGold, 1.7f, 3.f, sf::Color(8, 20, 40));
}

void MenuScreen::drawButton(const Button& button, bool hovered, float time) {
    const sf::FloatRect area = button.box.getGlobalBounds();
    drawPanel(area, hovered, time);

    sf::Text label(font, button.label, 22);
    label.setLetterSpacing(1.3f);
    label.setFillColor(hovered ? sf::Color::White : kInk);
    label.setPosition({area.position.x + 34.f, area.position.y + 9.f});
    window.draw(label);
}

void MenuScreen::drawMapCard(const MapCard& card, bool hovered, float time) {
    const sf::FloatRect area = card.box.getGlobalBounds();
    const float centerX = area.position.x + area.size.x / 2.f;
    drawPanel(area, hovered, time);

    sf::RectangleShape header({area.size.x, 34.f});
    header.setPosition(area.position);
    header.setFillColor(withAlpha(hovered ? kGold : kInkSoft, hovered ? 210.f : 70.f));
    window.draw(header);

    drawCenteredText(card.name, centerX, area.position.y + 6.f, 20, hovered ? sf::Color(12, 30, 52) : kInk, 1.6f);

    // Miniatura do tabuleiro, com escala real de cada mapa.
    const float preview = area.size.x - 56.f;
    const sf::Vector2f origin{area.position.x + 28.f, area.position.y + 62.f};
    const float cell = preview / card.grid;

    sf::RectangleShape water({preview, preview});
    water.setPosition(origin);
    water.setFillColor(sf::Color(10, 34, 58));
    water.setOutlineThickness(1.f);
    water.setOutlineColor(withAlpha(kInkSoft, 120.f));
    window.draw(water);

    for (int i = 1; i < card.grid; ++i) {
        const float offset = i * cell;
        drawLine(window, {origin.x + offset, origin.y}, {origin.x + offset, origin.y + preview},
                 withAlpha(kInkSoft, 55.f));
        drawLine(window, {origin.x, origin.y + offset}, {origin.x + preview, origin.y + offset},
                 withAlpha(kInkSoft, 55.f));
    }

    sf::CircleShape splash(cell * 0.22f, 12);
    splash.setOrigin({splash.getRadius(), splash.getRadius()});
    splash.setPosition({origin.x + preview - cell * 1.5f, origin.y + preview - cell * 1.5f});
    splash.setFillColor(sf::Color::Transparent);
    splash.setOutlineThickness(1.5f);
    splash.setOutlineColor(withAlpha(sf::Color(160, 220, 245), 170.f));
    window.draw(splash);

    const float footerY = origin.y + preview + 14.f;
    drawCenteredText(card.size, centerX, footerY, 18, kGold);
    drawCenteredText(card.fleet, centerX, footerY + 26.f, 13, hovered ? kInk : kInkSoft);
}

MenuOption MenuScreen::showMainMenu() {
    const float w = static_cast<float>(window.getSize().x);
    const float buttonWidth = 320.f;
    const float buttonHeight = 58.f;
    const float left = (w - buttonWidth) / 2.f;

    const std::vector<std::string> labels = {"Iniciar Jogo", "Ranking", "Instrucoes", "Sair"};
    const MenuOption results[4] = {MenuOption::START, MenuOption::RANKING,
                                   MenuOption::INSTRUCTIONS, MenuOption::EXIT};

    std::vector<Button> buttons;
    std::vector<sf::FloatRect> areas;
    for (std::size_t i = 0; i < labels.size(); ++i) {
        Button button;
        button.box.setSize({buttonWidth, buttonHeight});
        button.box.setPosition({left, 200.f + i * 74.f});
        button.label = labels[i];
        areas.push_back(button.box.getGlobalBounds());
        buttons.push_back(button);
    }

    while (window.isOpen()) {
        const int clicked = pollClick(areas);
        if (clicked >= 0) return results[clicked];

        const float time = animation.getElapsedTime().asSeconds();
        const sf::Vector2i mouse = sf::Mouse::getPosition(window);
        const sf::Vector2f cursor{(float)mouse.x, (float)mouse.y};

        window.clear(kSkyTop);
        drawSeascape();
        drawChartGrid();
        drawTitle();

        for (const Button& button : buttons)
            drawButton(button, button.box.getGlobalBounds().contains(cursor), time);

        drawCenteredText("Clique em uma opcao para comecar", w / 2.f, 556.f, 13, withAlpha(kInkSoft, 190.f), 1.6f);
        drawHudFrame();
        window.display();
    }
    return MenuOption::EXIT;
}

MapType MenuScreen::showMapSelection() {
    const float w = static_cast<float>(window.getSize().x);
    const float cardWidth = 218.f;
    const float cardHeight = 306.f;
    const float gap = 26.f;
    const float startX = (w - (3 * cardWidth + 2 * gap)) / 2.f;

    std::vector<MapCard> cards(3);

    cards[0].name = "ACUDE";   cards[0].size = "5 x 5";
    cards[0].fleet = "3 navios";  cards[0].grid = 5;  cards[0].type = MapType::ACUDE;

    cards[1].name = "LAGO";    cards[1].size = "8 x 8";
    cards[1].fleet = "5 navios";    cards[1].grid = 8;  cards[1].type = MapType::LAGO;

    cards[2].name = "OCEANO";  cards[2].size = "10 x 10";
    cards[2].fleet = "7 navios"; cards[2].grid = 10; cards[2].type = MapType::OCEANO;

    std::vector<sf::FloatRect> areas;
    for (std::size_t i = 0; i < cards.size(); ++i) {
        cards[i].box.setSize({cardWidth, cardHeight});
        cards[i].box.setPosition({startX + i * (cardWidth + gap), 168.f});
        areas.push_back(cards[i].box.getGlobalBounds());
    }

    while (window.isOpen()) {
        const int clicked = pollClick(areas);
        if (clicked >= 0) return cards[clicked].type;

        const float time = animation.getElapsedTime().asSeconds();
        const sf::Vector2i mouse = sf::Mouse::getPosition(window);
        const sf::Vector2f cursor{(float)mouse.x, (float)mouse.y};

        window.clear(kSkyTop);
        drawSeascape();
        drawChartGrid();

        drawCenteredText("ESCOLHA O CAMPO DE BATALHA", w / 2.f, 62.f, 32, kGold, 1.6f);
        drawCenteredText("Quanto maior o mar, maior a frota e o desafio", w / 2.f, 110.f, 14, kInkSoft, 1.8f);

        for (const MapCard& card : cards)
            drawMapCard(card, card.box.getGlobalBounds().contains(cursor), time);

        drawCenteredText("Clique em um mapa para zarpar", w / 2.f, 526.f, 13, withAlpha(kInkSoft, 190.f), 1.6f);
        drawHudFrame();
        window.display();
    }
    return MapType::OCEANO; // fallback se a janela fechar sem selecao
}
