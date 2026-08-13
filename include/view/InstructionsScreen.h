#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Tela de instrucoes: explica objetivo, controles e regras do jogo
 * antes da partida comecar.
 * @details Segue o mesmo padrao visual das demais telas (MenuScreen,
 * RankingScreen...), reutilizando o cenario e os elementos compartilhados
 * em UiTheme.
 */
class InstructionsScreen {
public:
    explicit InstructionsScreen(sf::RenderWindow& window);

    /// Mostra a tela e bloqueia ate o jogador voltar (ESC, clique ou fechar a janela).
    void show();

private:
    sf::RenderWindow& window;
    sf::Font font;
};
