#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

/**
 * @brief Paleta de cores e elementos decorativos compartilhados entre as
 * telas do jogo (menu, selecao de mapa, posicionamento...).
 * @details Extraido de MenuScreen para que outras telas (como
 * PlacementScreen) usem exatamente o mesmo visual "carta nautica",
 * evitando duplicar codigo e divergencia de estilo entre telas.
 */
namespace ui {

/// Resolucao fixa da janela do jogo. A janela nao e redimensionavel (sem
/// Resize/Fullscreen no estilo) justamente para evitar que o conteudo fique
/// cortado: como nenhuma tela reage a sf::Event::Resized, um redimensionamento
/// deixaria a View (fixa no tamanho de criacao) dessincronizada do tamanho
/// real da janela.
constexpr unsigned kWindowWidth = 1100;
constexpr unsigned kWindowHeight = 750;

extern const sf::Color kSkyTop;
extern const sf::Color kPanel;
extern const sf::Color kPanelHot;
extern const sf::Color kGold;
extern const sf::Color kInk;
extern const sf::Color kInkSoft;
extern const sf::Color kShadow;

sf::Color withAlpha(sf::Color color, float alpha);

void drawLine(sf::RenderWindow& window, sf::Vector2f a, sf::Vector2f b, sf::Color color);

void drawSeascape(sf::RenderWindow& window);

//grade
void drawChartGrid(sf::RenderWindow& window);

//moldura
void drawHudFrame(sf::RenderWindow& window);

//texto
void drawCenteredText(sf::RenderWindow& window,
                      const sf::Font& font,
                      const std::string& text,
                      float centerX,
                      float y,
                      unsigned size,
                      sf::Color color,
                      float letterSpacing = 1.f,
                      float outlineThickness = 0.f,
                      sf::Color outlineColor = sf::Color::Transparent);

//navio: casco em formato de capsula entre dois pontos (usado no posicionamento
//e nas miniaturas de mapa), com faixa de conves e juntas entre segmentos.
void drawShipHull(sf::RenderWindow& window,
                  sf::Vector2f startCenter,
                  sf::Vector2f endCenter,
                  bool shipHorizontal,
                  float radius,
                  sf::Color hullColor,
                  sf::Color deckColor,
                  int segments,
                  std::uint8_t alpha = 255);

} // namespace ui
