#pragma once

#include <MapLayout.h>

#include <SFML/Graphics.hpp>

#include <string>

/**
 * @brief Opções que o jogador pode escolher no menu principal.
 */
enum class MenuOption {
    START,
    RANKING,
    INSTRUCTIONS,
    EXIT        
};

/**
 * @brief Tela de menu principal e de seleção de mapa.
 * @details Desenha o visual "carta náutica" compartilhado com as demais
 * telas (via UiTheme) e expõe dois fluxos bloqueantes: showMainMenu(), com
 * os botões do menu (Iniciar Jogo/Ranking/Como Jogar/Sair), e
 * showMapSelection(), com os cartões dos três mapas disponíveis.
 *
 * @author Suelle
 * @date 16/07/2026
 */
class MenuScreen {
public:
    /**
     * @brief Constrói a tela de menu.
     * @details Carrega a fonte usada nos textos da tela.
     * @param window Janela principal da aplicação.
     * @throws std::runtime_error se a fonte não puder ser carregada.
     */
    explicit MenuScreen(sf::RenderWindow& window);

    /**
     * @brief Mostra o menu principal e bloqueia até o jogador escolher uma opção.
     * @details Desenha o título e os botões (Iniciar Jogo/Ranking/Como
     * Jogar/Sair) em loop, com efeito de destaque no botão sob o cursor,
     * até que algum deles seja clicado ou a janela seja fechada.
     * @return A MenuOption escolhida, ou MenuOption::EXIT se a janela for fechada.
     */
    MenuOption showMainMenu();

    /**
     * @brief Mostra a tela de seleção de mapa e bloqueia até o jogador escolher um.
     * @details Desenha os três cartões de mapa (Açude 5x5, Lago 8x8 e
     * Oceano 10x10), cada um com uma miniatura do tabuleiro, em loop, até
     * que algum seja clicado ou a janela seja fechada.
     * @return O MapType escolhido, ou MapType::OCEANO se a janela for fechada.
     */
    MapType showMapSelection();

private:
    struct Button {
        sf::RectangleShape box; 
        std::string label;    
    };

    struct MapCard {
        sf::RectangleShape box; 
        std::string name;    
        std::string size;      
        std::string fleet;      
        int grid;     
        MapType type;            
    };

    /**
     * @brief Desenha o título "BATALHA NAVAL", com uma linha dourada
     * decorativa e um subtítulo logo abaixo.
     */
    void drawTitle();

    /**
     * @brief Desenha o painel de fundo compartilhado por botões e cartões.
     * @details Composto por um brilho pulsante ao redor da área quando
     * hovered, uma sombra projetada e a própria placa (preenchimento
     * kPanel/kPanelHot, com borda dourada quando hovered).
     * @param area Retângulo (posição e tamanho) do painel.
     * @param hovered true se o cursor do mouse estiver sobre a área.
     * @param time Tempo decorrido (em segundos) usado para animar o brilho pulsante.
     */
    void drawPanel(sf::FloatRect area, bool hovered, float time);

    /**
     * @brief Desenha um botão do menu principal.
     * @param button Botão a ser desenhado (área e texto).
     * @param hovered true se o cursor do mouse estiver sobre o botão.
     * @param time Tempo decorrido (em segundos), repassado a drawPanel() para a animação.
     */
    void drawButton(const Button& button, bool hovered, float time);

    /**
     * @brief Desenha um cartão de seleção de mapa.
     * @details Além do painel de fundo, desenha o cabeçalho com o nome do
     * mapa, uma miniatura do tabuleiro (grade, dois navios de exemplo e uma
     * marca de respingo) e o rodapé com a dimensão e o tamanho da frota.
     * @param card Cartão a ser desenhado (área e dados do mapa).
     * @param hovered true se o cursor do mouse estiver sobre o cartão.
     * @param time Tempo decorrido (em segundos), repassado a drawPanel() para a animação.
     */
    void drawMapCard(const MapCard& card, bool hovered, float time);

    /**
     * @brief Desenha um texto centralizado horizontalmente em torno de um ponto X.
     * @details Repassa os parâmetros para ui::drawCenteredText(), já
     * informando a janela e a fonte da tela.
     * @param text Conteúdo do texto a ser exibido.
     * @param centerX Posição X (em pixels) em torno da qual o texto será centralizado.
     * @param y Posição Y (em pixels) do topo do texto.
     * @param size Tamanho da fonte, em pixels.
     * @param color Cor de preenchimento do texto.
     * @param letterSpacing Multiplicador de espaçamento entre letras (1.f = espaçamento padrão da fonte).
     * @param outlineThickness Espessura do contorno do texto (0.f = sem contorno).
     * @param outlineColor Cor do contorno do texto, usada apenas se outlineThickness for maior que 0.
     */
    void drawCenteredText(const std::string& text,
                          float centerX,
                          float y,
                          unsigned size,
                          sf::Color color,
                          float letterSpacing = 1.f,
                          float outlineThickness = 0.f,
                          sf::Color outlineColor = sf::Color::Transparent);

    int pollClick(const std::vector<sf::FloatRect>& areas);

    sf::RenderWindow& window; 
    sf::Font font; 
    sf::Clock animation;
};
