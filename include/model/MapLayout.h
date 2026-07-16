#pragma once
#include "Board.h"

/**
 * @brief Identifica o mapa selecionado pelo jogador
 * @details Determina as dimensões do tabuleiro, a composição da frota
 * e o layout de terrenos aplicado pelo MapLayout.
 *
 * | MapType | Dimensão | Frota                        |
 * |---------|----------|------------------------------|
 * | Açude   | 5 × 5    | 1P + 1M + 1G  (3 navios)     |
 * | LAGO    | 8 × 8    | 2P + 2M + 1G  (5 navios)     |
 * | OCEANO  | 10 × 10  | 3P + 2M + 2G  (7 navios)     |
 *
 * @author Georis
 * @date 17/07/2026
 */
enum class MapType{ACUDE, LAGO, OCEANO};

/**
 * @brief Aplica o layout de terreno de cada mapa nos tabuleiros
 * @details MapLayout é uma classe utilitária (sem estado) que define
 * onde ficam células BLOCKED e SHALLOW em cada tipo de mapa.
 * Deve ser chamado uma única vez pelo GameState após criar os boards,
 * antes de qualquer posicionamento de navios.
 *
 * Por que uma classe separada e não métodos no Board?
 * O Board não sabe nada sobre mapas, ele só armazena células.
 * A decisão de qual terreno fica em qual posição é responsabilidade
 * do domínio do jogo, não da estrutura de dados.
 *
 * @author Georis
 * @date 17/07/2026
 */
class MapLayout {
public:
    /**
     * @brief Aplica o terreno correto no board conforme o mapa escolhido
     * @details Chama setCell() no board para marcar células como BLOCKED
     * ou SHALLOW conforme o design de cada mapa.
     * Deve ser aplicado nos dois boards (jogador e computador) para que as regras de terreno sejam simétricas.
     *
     * @param board Referência ao tabuleiro onde o terreno será aplicado.
     * @param map   Tipo do mapa selecionado pelo jogador.
     *
     * @warning Chamar apply() após navios já posicionados pode corromper
     * o estado do tabuleiro. Sempre aplique antes da fase de posicionamento.
     *
     * @see Board::setCell()
     * @see GameState::GameState(MapType)
     *
     * @author Georis
     * @date 17/07/2026
     */
    static void apply(Board& board, MapType map);
};