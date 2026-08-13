#include "GameController.h"
#include <random>

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

// --- Gerenciamento de Armas (RF02) ---

void GameController::selectWeapon(WeaponType weapon) {
    if (weapon == WeaponType::MISSILE && missilesLeft <= 0) return;
    if (weapon == WeaponType::TORPEDO && torpedoesLeft <= 0) return;
    selectedWeapon = weapon;
}

WeaponType GameController::getSelectedWeapon() const { return selectedWeapon; }
int GameController::getMissilesLeft() const { return missilesLeft; }
int GameController::getTorpedoesLeft() const { return torpedoesLeft; }

WeaponType GameController::awardRandomWeapon() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> coin(0, 1);

    const WeaponType awarded = coin(generator) == 0 ? WeaponType::MISSILE : WeaponType::TORPEDO;
    if (awarded == WeaponType::MISSILE) missilesLeft++;
    else torpedoesLeft++;

    return awarded;
}

// --- Processamento de Disparos (RF02) ---

CellState GameController::fireBasic(Board& enemy, int row, int col) {
    if (!enemy.isValidShot(row, col))
        return enemy.getCell(row, col);
    return enemy.shoot(row, col);
}

int GameController::fireMissile(Board& enemy, int row, int col) {
    if (missilesLeft <= 0) return 0;

    int hits = 0;
    // Quadrante 2x2 a partir do canto superior esquerdo clicado
    for (int dr = 0; dr <= 1; dr++) {
        for (int dc = 0; dc <= 1; dc++) {
            int r = row + dr;
            int c = col + dc;
            if (enemy.isValidShot(r, c)) {
                if (enemy.shoot(r, c) == CellState::HIT) hits++;
            }
        }
    }

    missilesLeft--;
    selectedWeapon = WeaponType::BASIC; // Volta para o disparo básico
    return hits;
}

int GameController::fireTorpedo(Board& enemy, int row, int col) {
    if (torpedoesLeft <= 0) return 0;

    int hits = 0;
    // Percorre todas as colunas da linha especificada
    for (int c = 0; c < enemy.getCols(); c++) {
        if (enemy.isValidShot(row, c)) {
            if (enemy.shoot(row, c) == CellState::HIT) hits++;
        }
    }

    torpedoesLeft--;
    selectedWeapon = WeaponType::BASIC; // Volta para o disparo básico
    return hits;
}

GameController::FireResult GameController::fire(Board& enemy, std::vector<Ship>& enemyShips, int row, int col) {
    FireResult result;

    // Monta a lista de células-alvo de acordo com a arma selecionada:
    // BASIC atinge só a célula clicada, MISSILE o quadrante 2x2 a partir
    // dela e TORPEDO a linha inteira.
    std::vector<std::pair<int, int>> targets;
    switch (selectedWeapon) {
        case WeaponType::MISSILE:
            for (int dr = 0; dr <= 1; dr++)
                for (int dc = 0; dc <= 1; dc++)
                    targets.push_back({row + dr, col + dc});
            break;
        case WeaponType::TORPEDO:
            for (int c = 0; c < enemy.getCols(); c++)
                targets.push_back({row, c});
            break;
        case WeaponType::BASIC:
        default:
            targets.push_back({row, col});
            break;
    }

    for (const auto& [r, c] : targets) {
        if (!enemy.isValidShot(r, c)) continue;

        if (enemy.shoot(r, c) == CellState::HIT) {
            result.hits++;
            for (Ship& ship : enemyShips) {
                if (ship.occupiesCell(r, c)) {
                    ship.hit();
                    if (ship.isSunk()) result.shipsSunk++;
                    break;
                }
            }
        } else {
            result.misses++;
        }
    }

    if (selectedWeapon == WeaponType::MISSILE) missilesLeft--;
    if (selectedWeapon == WeaponType::TORPEDO) torpedoesLeft--;
    selectedWeapon = WeaponType::BASIC; // Volta para o disparo básico após qualquer disparo.

    return result;
}
