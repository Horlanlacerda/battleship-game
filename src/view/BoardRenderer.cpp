#include "BoardRenderer.h"

BoardRenderer::BoardRenderer(sf::RenderWindow& window,
                              float cellSize, float offsetX, float offsetY)
    : window(window), cellSize(cellSize), offsetX(offsetX), offsetY(offsetY)
{
    // Carrega a fonte usada para os rotulos de coordenadas (A1, B3...)
    font.openFromFile("assets/fonts/Roboto-Regular.ttf");
}

sf::Color BoardRenderer::colorForState(CellState state, bool hideShips) const {
    // No tabuleiro inimigo, navio intacto nao deve ser revelado visualmente
    if (state == CellState::SHIP && hideShips)
        return sf::Color(20, 50, 100);

    switch (state) {
        case CellState::EMPTY:   return sf::Color(20,  50, 100);  // azul escuro
        case CellState::SHIP:    return sf::Color(60, 120, 200);  // azul medio
        case CellState::HIT:     return sf::Color(200, 60,  40);  // vermelho
        case CellState::MISS:    return sf::Color(150,200, 230);  // azul claro
        case CellState::BLOCKED: return sf::Color(80,  80,  80);  // cinza escuro
        case CellState::SHALLOW: return sf::Color(180,160, 100);  // areia
        default:                 return sf::Color::Black;
    }
}

void BoardRenderer::draw(const Board& board, bool hideShips) {
    for (int r = 0; r < board.getRows(); r++) {
        for (int c = 0; c < board.getCols(); c++) {

            sf::RectangleShape cell({cellSize - 2, cellSize - 2});
            cell.setPosition({offsetX + c * cellSize, offsetY + r * cellSize});
            cell.setFillColor(colorForState(board.getCell(r, c), hideShips));
            cell.setOutlineColor(sf::Color(10, 30, 60));
            cell.setOutlineThickness(1.f);
            window.draw(cell);

            // Rotulos de coordenada: letras no topo, numeros na lateral
            if (r == 0) {
                sf::Text col_label(font, std::string(1, 'A' + c), 14);
                col_label.setPosition({offsetX + c * cellSize + 16, offsetY - 20});
                col_label.setFillColor(sf::Color::White);
                window.draw(col_label);
            }
            if (c == 0) {
                sf::Text row_label(font, std::to_string(r + 1), 14);
                row_label.setPosition({offsetX - 20, offsetY + r * cellSize + 14});
                row_label.setFillColor(sf::Color::White);
                window.draw(row_label);
            }
        }
    }
}