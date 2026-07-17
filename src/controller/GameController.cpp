#include "GameController.h"

GameController::GameController(float cellSize, float offsetX, float offsetY, int rows, int cols)
    : cellSize(cellSize), offsetX(offsetX), offsetY(offsetY), rows(rows), cols(cols) {}

bool GameController::screenToBoardCoord(sf::Vector2i mousePos, int& outRow, int& outCol) const {
    // Traduz pixels para indice de matriz, invertendo a formula usada no BoardRenderer
    float relativeX = static_cast<float>(mousePos.x) - offsetX;
    float relativeY = static_cast<float>(mousePos.y) - offsetY;

    // Clique antes do inicio do tabuleiro (esquerda/cima da grade)
    if (relativeX < 0 || relativeY < 0)
        return false;

    int col = static_cast<int>(relativeX / cellSize);
    int row = static_cast<int>(relativeY / cellSize);

    // Clique depois do fim do tabuleiro (direita/baixo da grade)
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return false;

    outRow = row;
    outCol = col;
    return true;
}