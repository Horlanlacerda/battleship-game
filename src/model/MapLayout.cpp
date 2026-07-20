#include "MapLayout.h"

/*
 * Define o layout de terreno de cada mapa.
 * As posições foram escolhidas para
 *   > Não bloquear o posicionamento de toda a frota obrigatória
 *   > Ser visualmente identificáveis pelo BoardRenderer (Sprint 4)
 *
 * Guia de leitura: setCell(row, col, estado)
 *   row 0 = linha de cima, col 0 = coluna da esquerda.
 */
void MapLayout::apply(Board& board, MapType map) {

    if (map == MapType::ACUDE) {
        // ---- Açude 5×5 --------------------------------------->
        // Tabuleiro pequeno, ou seja, terreno mínimo para não sufocar a frota de 3 navios.
        //
        // Layout visual (B = BLOCKED, S = SHALLOW, . = EMPTY):
        //   . . . . .
        //   . . . . .
        //   . . B . .   ← ilha no centro
        //   . . . . .
        //   S . . . S   ← recifes nos cantos inferiores

        board.setCell(2, 2, CellState::BLOCKED); // ilha central

        board.setCell(4, 0, CellState::SHALLOW); // recife canto inferior esquerdo
        board.setCell(4, 4, CellState::SHALLOW); // recife canto inferior direito
    }

    else if (map == MapType::LAGO) {
        // --- Lago 8×8 ---------------------------------------->
        // Tabuleiro médio com ilha dupla no centro + recifes nas bordas.
        //
        // Layout visual:
        //   . . . . . . . .
        //   . . . . . . . .
        //   . . . . . . . .
        //   . . . B B . . .   ← ilha dupla no centro
        //   . . . B B . . .
        //   . . . . . . . .
        //   S . . . . . . S   ← recifes laterais
        //   . . . . . . . .

        // Ilha 2×2 no centro (linhas 3-4, colunas 3-4)
        board.setCell(3, 3, CellState::BLOCKED);
        board.setCell(3, 4, CellState::BLOCKED);
        board.setCell(4, 3, CellState::BLOCKED);
        board.setCell(4, 4, CellState::BLOCKED);

        // Recifes rasos nas bordas laterais
        board.setCell(6, 0, CellState::SHALLOW);
        board.setCell(6, 7, CellState::SHALLOW);
    }

    else if (map == MapType::OCEANO) {
        // --- Oceano 10×10 ---------------------------------->
        //
        // Layout visual (posições aproximadas):
        //   . . . . . . . . . .
        //   . . . . . . . . . .
        //   . . . . . . . . . .
        //   . . . . . . . . . .
        //   . . . . B B . . . .   ← ilha central (2 células)
        //   . . . . B B . . . .
        //   . . . . . . . . . .
        //   S S . . . . . . S S   ← faixa de recife nas bordas
        //   . . . . . . . . . .
        //   . . . . . . . . . .

        // Ilha 2×2 no centro
        board.setCell(4, 4, CellState::BLOCKED);
        board.setCell(4, 5, CellState::BLOCKED);
        board.setCell(5, 4, CellState::BLOCKED);
        board.setCell(5, 5, CellState::BLOCKED);

        // Recifes rasos: faixa horizontal na linha 7, bordas esquerda e direita
        board.setCell(7, 0, CellState::SHALLOW);
        board.setCell(7, 1, CellState::SHALLOW);
        board.setCell(7, 8, CellState::SHALLOW);
        board.setCell(7, 9, CellState::SHALLOW);
    }
}