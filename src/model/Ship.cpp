#include "Ship.h"

Ship::Ship(ShipType type, int startRow, int startCol, bool horizontal)
    : type(type),
      startRow(startRow),
      startCol(startCol),
      size(sizeForType(type)),
      horizontal(horizontal),
      hitsReceived(0) {}

ShipType Ship::getType() const { return type; }
int Ship::getSize() const { return size; }
bool Ship::isHorizontal() const { return horizontal; }
int Ship::getStartRow() const { return startRow; }
int Ship::getStartCol() const { return startCol; }

std::vector<std::pair<int, int>> Ship::getOccupiedCells() const {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(size);

    // Se horizontal, a coluna varia; se vertical, a linha varia.
    for (int i = 0; i < size; i++) {
        int row = horizontal ? startRow : startRow + i;
        int col = horizontal ? startCol + i : startCol;
        cells.emplace_back(row, col);
    }

    return cells;
}

bool Ship::occupiesCell(int row, int col) const {
    // Busca linear simples — navios têm no máximo 5 células, custo irrelevante.
    for (const auto& cell : getOccupiedCells()) {
        if (cell.first == row && cell.second == col) {
            return true;
        }
    }
    return false;
}

void Ship::setPosition(int row, int col, bool isHorizontal) {
    startRow = row;
    startCol = col;
    horizontal = isHorizontal;
}

void Ship::hit() {
    // Trava de segurança: nunca ultrapassa o tamanho do navio.
    if (hitsReceived < size) {
        hitsReceived++;
    }
}

bool Ship::isSunk() const {
    return hitsReceived >= size;
}

int Ship::getRemainingSegments() const {
    return size - hitsReceived;
}

std::string Ship::getTypeName() const {
    switch (type) {
        case ShipType::SMALL:  return "Pequeno";
        case ShipType::MEDIUM: return "Medio";
        case ShipType::LARGE:  return "Grande";
        default:               return "Desconhecido";
    }
}

int Ship::sizeForType(ShipType type) {
    // Regra central de tamanhos da frota: mudar aqui basta.
    switch (type) {
        case ShipType::SMALL:  return 2;
        case ShipType::MEDIUM: return 3;
        case ShipType::LARGE:  return 5;
        default:                return 0;
    }
}