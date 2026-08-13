#include "utils/UiTheme.h"

#include <algorithm>
#include <cstdint>

namespace ui {

const sf::Color kSkyTop   ( 18,  34,  62);
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

void drawChartGrid(sf::RenderWindow& window) {
    const float w = static_cast<float>(window.getSize().x);
    const float h = static_cast<float>(window.getSize().y);
    const sf::Color grid = withAlpha(sf::Color(140, 200, 230), 16.f);

    for (float x = 0.f; x <= w; x += 40.f)
        drawLine(window, {x, 0.f}, {x, h}, grid);
    for (float y = 0.f; y <= h; y += 40.f)
        drawLine(window, {0.f, y}, {w, y}, grid);
}

void drawHudFrame(sf::RenderWindow& window) {
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

void drawCenteredText(sf::RenderWindow& window,
                      const sf::Font& font,
                      const std::string& text,
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

namespace {

// Duas circulos ligados por um retangulo, formando um contorno arredondado
// entre dois pontos. Base geometrica usada por drawShipHull().
void drawCapsule(sf::RenderWindow& window, sf::Vector2f startCenter, sf::Vector2f endCenter,
                 bool shipHorizontal, float radius, sf::Color color) {
    sf::CircleShape capStart(radius, 24);
    capStart.setOrigin({radius, radius});
    capStart.setPosition(startCenter);
    capStart.setFillColor(color);
    window.draw(capStart);

    sf::CircleShape capEnd(radius, 24);
    capEnd.setOrigin({radius, radius});
    capEnd.setPosition(endCenter);
    capEnd.setFillColor(color);
    window.draw(capEnd);

    sf::RectangleShape barrel;
    if (shipHorizontal) {
        barrel.setSize({endCenter.x - startCenter.x, radius * 2.f});
        barrel.setOrigin({0.f, radius});
    } else {
        barrel.setSize({radius * 2.f, endCenter.y - startCenter.y});
        barrel.setOrigin({radius, 0.f});
    }
    barrel.setPosition(startCenter);
    barrel.setFillColor(color);
    window.draw(barrel);
}

} // namespace

void drawShipHull(sf::RenderWindow& window, sf::Vector2f startCenter, sf::Vector2f endCenter,
                  bool shipHorizontal, float radius, sf::Color hullColor, sf::Color deckColor,
                  int segments, std::uint8_t alpha) {
    hullColor.a = alpha;
    deckColor.a = alpha;

    // Contorno: mesma capsula, um pouco maior e mais escura, atras do casco.
    const sf::Color outline(8, 20, 40, alpha);
    drawCapsule(window, startCenter, endCenter, shipHorizontal, radius + 2.5f, outline);
    drawCapsule(window, startCenter, endCenter, shipHorizontal, radius, hullColor);

    // Faixa do conves, ao longo do eixo maior do navio.
    const float deckThickness = radius * 0.9f;
    sf::RectangleShape deck;
    if (shipHorizontal) {
        deck.setSize({endCenter.x - startCenter.x, deckThickness});
        deck.setOrigin({0.f, deckThickness / 2.f});
    } else {
        deck.setSize({deckThickness, endCenter.y - startCenter.y});
        deck.setOrigin({deckThickness / 2.f, 0.f});
    }
    deck.setPosition(startCenter);
    deck.setFillColor(deckColor);
    window.draw(deck);

    // Uma marca de junta entre cada par de celulas do navio.
    const sf::Color seam(8, 20, 40, static_cast<std::uint8_t>(alpha / 2));
    for (int i = 1; i < segments; i++) {
        const float t = static_cast<float>(i) / static_cast<float>(segments);
        const sf::Vector2f p{startCenter.x + (endCenter.x - startCenter.x) * t,
                             startCenter.y + (endCenter.y - startCenter.y) * t};

        if (shipHorizontal)
            drawLine(window, {p.x, p.y - radius * 0.7f}, {p.x, p.y + radius * 0.7f}, seam);
        else
            drawLine(window, {p.x - radius * 0.7f, p.y}, {p.x + radius * 0.7f, p.y}, seam);
    }
}

} // namespace ui
