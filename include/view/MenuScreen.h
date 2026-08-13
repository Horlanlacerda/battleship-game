    #pragma once

    #include <MapLayout.h>

    #include <SFML/Graphics.hpp>

    #include <string>

    enum class MenuOption { START, RANKING, INSTRUCTIONS, EXIT };

    class MenuScreen {
    public:
        explicit MenuScreen(sf::RenderWindow& window);
        MenuOption showMainMenu();
        MapType showMapSelection();

    private:
        /// Botao do menu: area clicavel + texto exibido dentro dela.
        struct Button {
            sf::RectangleShape box;
            std::string label;
        };

        /// Cartao de selecao de mapa: area clicavel + dados do tabuleiro.
        struct MapCard {
            sf::RectangleShape box;
            std::string name;
            std::string size;
            std::string fleet;
            int grid;      ///< Lado do tabuleiro, usado na miniatura.
            MapType type;
        };

        // Cenario (tudo desenhado por codigo, sem imagens externas).
        void drawSeascape();
        void drawChartGrid();
        void drawHudFrame();

        // Elementos de interface.
        void drawTitle();
        void drawPanel(sf::FloatRect area, bool hovered, float time);
        void drawButton(const Button& button, bool hovered, float time);
        void drawMapCard(const MapCard& card, bool hovered, float time);
        void drawCenteredText(const std::string& text,
                              float centerX,
                              float y,
                              unsigned size,
                              sf::Color color,
                              float letterSpacing = 1.f,
                              float outlineThickness = 0.f,
                              sf::Color outlineColor = sf::Color::Transparent);

        /// Consome os eventos pendentes; fecha a janela em Closed e retorna o indice
        /// da area clicada com o botao esquerdo, ou -1 se nao houve clique.
        int pollClick(const std::vector<sf::FloatRect>& areas);

        sf::RenderWindow& window;
        sf::Font font;
        sf::Clock animation;   ///< Tempo continuo que alimenta as animacoes.
    };
