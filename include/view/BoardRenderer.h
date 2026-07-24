#pragma once

#include <SFML/Graphics.hpp>
#include "../model/Board.h"

/**
 * @brief Responsável por desenhar o tabuleiro na tela usando SFML.
 * @details A classe BoardRenderer não processa eventos nem modifica dados —
 * ela apenas lê o estado atual de um Board e o renderiza numa janela SFML.
 * Nota de arquitetura: mantém a separação MVC, já que a lógica do jogo
 * (Board, Ship) não conhece nada sobre SFML ou renderização.
 * @author Gabi
 */
class BoardRenderer {
public:
    /**
     * @brief Constrói um renderer associado a uma janela SFML.
     * @param window Referência para a janela onde o tabuleiro será desenhado.
     * @param cellSize Tamanho em pixels de cada célula do tabuleiro.
     * @param offsetX Deslocamento horizontal (em pixels) de onde o tabuleiro começa a ser desenhado.
     * @param offsetY Deslocamento vertical (em pixels) de onde o tabuleiro começa a ser desenhado.
     */
    BoardRenderer(sf::RenderWindow& window, float cellSize,
                  float offsetX, float offsetY);

    /**
     * @brief Desenha o estado atual do tabuleiro na janela.
     * @param board O tabuleiro a ser renderizado.
     * @param hideShips Se true, esconde navios (usado no tabuleiro do adversário,
     * para que o jogador não veja onde os navios inimigos estão).
     */
    void draw(const Board& board, bool hideShips = false);

private:
    sf::RenderWindow& window;
    float cellSize, offsetX, offsetY;
    sf::Font font;

    /**
     * @brief Determina a cor de uma célula com base em seu estado.
     * @param state Estado atual da célula.
     * @param hideShips Se true, células SHIP são desenhadas como EMPTY visualmente.
     * @return A cor SFML correspondente ao estado da célula.
     */
    sf::Color colorForState(CellState state, bool hideShips) const;
};