#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Converte cliques do mouse em coordenadas de tabuleiro (linha, coluna).
 * @details O GameController não processa regras de jogo nem desenha nada —
 * ele só traduz posições de tela (pixels) para posições de matriz, usando
 * os mesmos parâmetros de tamanho/deslocamento que o BoardRenderer usa
 * para desenhar, garantindo que clique e desenho fiquem sincronizados.
 * @author Gabi
 */
class GameController {
public:
    /**
     * @brief Constrói o controller com os mesmos parâmetros visuais do tabuleiro.
     * @param cellSize Tamanho em pixels de cada célula (deve bater com o BoardRenderer).
     * @param offsetX Deslocamento horizontal onde o tabuleiro começa na tela.
     * @param offsetY Deslocamento vertical onde o tabuleiro começa na tela.
     * @param rows Número de linhas do tabuleiro (para validar limites).
     * @param cols Número de colunas do tabuleiro (para validar limites).
     */
    GameController(float cellSize, float offsetX, float offsetY, int rows, int cols);

    /**
     * @brief Converte uma posição de clique (em pixels) para coordenada de matriz.
     * @param mousePos Posição do clique em pixels, geralmente vinda de sf::Mouse::getPosition(window).
     * @param outRow Referência onde a linha calculada será armazenada, se o clique for válido.
     * @param outCol Referência onde a coluna calculada será armazenada, se o clique for válido.
     * @return true se o clique caiu dentro dos limites do tabuleiro; false caso contrário.
     */
    bool screenToBoardCoord(sf::Vector2i mousePos, int& outRow, int& outCol) const;

private:
    float cellSize, offsetX, offsetY;
    int rows, cols;
};