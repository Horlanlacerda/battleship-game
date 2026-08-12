#include "utils/UiTheme.h"

#include <algorithm>
#include <cstdint>

namespace ui {

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

void drawSeascape(sf::RenderWindow& window) {
    sf::CircleShape moonDisc(18.f);
    moonDisc.setOrigin({18.f, 18.f});
    moonDisc.setFillColor(sf::Color(226, 238, 250));
    window.draw(moonDisc);
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

} // namespace ui
