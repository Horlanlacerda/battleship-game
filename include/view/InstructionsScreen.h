#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Tela de instrucoes: explica objetivo, controles e regras do jogo
 * antes da partida comecar.
 * @details Segue o mesmo padrao visual das demais telas (MenuScreen,
 * RankingScreen...), reutilizando o cenario e os elementos compartilhados
 * em UiTheme.
 *
 * @author Suelle
 * @date 12/08/2026
 */
class InstructionsScreen {
public:
    /**
     * @brief Constrói a tela de instruções.
     * @details Carrega a fonte usada nos textos da tela.
     * @param window Janela principal da aplicação.
     * @throws std::runtime_error se a fonte não puder ser carregada.
     */
    explicit InstructionsScreen(sf::RenderWindow& window);

    /**
     * @brief Mostra a tela de instruções e bloqueia até o jogador voltar.
     * @details Desenha o título, um painel com as seções de regras
     * (objetivo, posicionamento, batalha, terreno e vitória) e a dica de
     * como sair. Roda em loop até a janela ser fechada, ESC ser pressionado
     * ou qualquer clique do mouse ser detectado.
     */
    void show();

private:
    sf::RenderWindow& window; 
    sf::Font font;         
};
