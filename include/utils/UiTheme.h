#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

/**
 * @brief Paleta de cores e elementos decorativos compartilhados entre as
 * telas do jogo (menu, selecao de mapa, posicionamento, etc).
 * @details Extraido de MenuScreen para que outras telas (como
 * PlacementScreen) usem exatamente o mesmo visual "carta nautica",
 * evitando duplicar codigo e divergencia de estilo entre telas.
 *
 * @author Suelle
 * @date 12/08/2026
 */
namespace ui {

/// Resolucao fixa da janela do jogo. A janela nao e redimensionavel para evitar que o conteúdo fique cortado
constexpr unsigned kWindowWidth = 1100;  ///< Largura fixa da janela, em pixels.
constexpr unsigned kWindowHeight = 750;  ///< Altura fixa da janela, em pixels.

extern const sf::Color kSkyTop;   
extern const sf::Color kPanel;    
extern const sf::Color kPanelHot;  
extern const sf::Color kGold;      
extern const sf::Color kInk;      
extern const sf::Color kInkSoft;  
extern const sf::Color kShadow;    

/**
 * @brief Retorna uma copia da cor informada com um novo canal alfa (transparencia).
 * @details Usado em toda a interface para variar a opacidade das cores da
 * paleta (ex.: kGold, kInkSoft) sem precisar declarar uma nova constante de
 * cor para cada nivel de transparencia desejado.
 * @param color Cor original (o canal alfa atual dela e ignorado/substituido).
 * @param alpha Novo valor do canal alfa, entre 0 (totalmente transparente) e
 * 255 (totalmente opaco). Valores fora desse intervalo sao limitados (clamp).
 * @return A cor informada com o canal alfa substituido.
 */
sf::Color withAlpha(sf::Color color, float alpha);

/**
 * @brief Desenha uma linha reta de 1 pixel entre dois pontos.
 * @details Bloco basico usado por drawChartGrid() (linhas da grade) e por
 * drawShipHull() (marcas de junta entre segmentos do navio).
 * @param window Janela onde a linha sera desenhada.
 * @param a Ponto inicial da linha, em coordenadas de tela.
 * @param b Ponto final da linha, em coordenadas de tela.
 * @param color Cor da linha.
 */
void drawLine(sf::RenderWindow& window, sf::Vector2f a, sf::Vector2f b, sf::Color color);

/**
 * @brief Desenha uma grade de linhas finas e semi-transparentes sobre toda a janela.
 * @details Reforca o visual de "carta nautica" das telas, com uma linha
 * vertical e uma horizontal a cada 40 pixels.
 * @param window Janela onde a grade sera desenhada.
 */
void drawChartGrid(sf::RenderWindow& window);

/**
 * @brief Desenha a moldura dourada decorativa ao redor de toda a janela.
 * @details Composta por uma borda fina semi-transparente proxima das bordas
 * da janela e por "cantoneiras" douradas mais solidas em cada um dos 4 cantos.
 * Chamado por ultimo em cada tela, para ficar por cima do restante do conteudo.
 * @param window Janela onde a moldura sera desenhada.
 */
void drawHudFrame(sf::RenderWindow& window);

/**
 * @brief Desenha um texto centralizado horizontalmente em torno de um ponto X.
 * @details Calcula os limites (bounds) do texto apos aplicar tamanho e
 * espacamento entre letras, e ajusta a origem para que ele fique centralizado
 * em centerX, evitando que cada tela precise calcular manualmente a largura
 * do texto para centraliza-lo.
 * @param window Janela onde o texto sera desenhado.
 * @param font Fonte usada para renderizar o texto.
 * @param text Conteudo do texto a ser exibido.
 * @param centerX Posicao X (em pixels) em torno da qual o texto sera centralizado.
 * @param y Posicao Y (em pixels) do topo do texto.
 * @param size Tamanho da fonte, em pixels.
 * @param color Cor de preenchimento do texto.
 * @param letterSpacing Multiplicador de espacamento entre letras (1.f = espacamento padrao da fonte).
 * @param outlineThickness Espessura do contorno do texto (0.f = sem contorno).
 * @param outlineColor Cor do contorno do texto, usada apenas se outlineThickness for maior que 0.
 */
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

/**
 * @brief Desenha o casco de um navio em formato de capsula entre dois pontos.
 * @details Usado tanto no posicionamento/batalha (PlacementScreen, GameScreen)
 * quanto nas miniaturas de mapa do MenuScreen. O casco e composto por duas
 * capsulas sobrepostas (um contorno escuro atras do preenchimento de
 * hullColor), uma faixa de conves na cor deckColor ao longo do eixo maior do
 * navio, e uma marca de junta entre cada par de celulas do navio.
 * @param window Janela onde o navio sera desenhado.
 * @param startCenter Centro, em pixels, da primeira celula ocupada pelo navio (a proa).
 * @param endCenter Centro, em pixels, da ultima celula ocupada pelo navio (a popa).
 * @param shipHorizontal true se o navio estiver na orientacao horizontal, false se vertical.
 * @param radius Raio das capsulas que formam o casco (define a "grossura" do navio).
 * @param hullColor Cor de preenchimento do casco.
 * @param deckColor Cor de preenchimento da faixa de conves.
 * @param segments Numero de celulas ocupadas pelo navio (define quantas marcas de junta sao desenhadas).
 * @param alpha Opacidade do navio inteiro (casco, conves e contorno), de 0 a 255. Usado para desenhar o preview semi-transparente do navio durante o posicionamento.
 */
void drawShipHull(sf::RenderWindow& window,
                  sf::Vector2f startCenter,
                  sf::Vector2f endCenter,
                  bool shipHorizontal,
                  float radius,
                  sf::Color hullColor,
                  sf::Color deckColor,
                  int segments,
                  std::uint8_t alpha = 255);

}
