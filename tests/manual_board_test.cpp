#include "../include/model/Board.h"
#include <iostream>

void printBoard(const Board& b) {
    for (int r = 0; r < b.getRows(); r++) {
        for (int c = 0; c < b.getCols(); c++) {
            switch (b.getCell(r, c)) {
                case CellState::EMPTY:   std::cout << ". "; break;
                case CellState::SHIP:    std::cout << "S "; break;
                case CellState::HIT:     std::cout << "X "; break;
                case CellState::MISS:    std::cout << "O "; break;
                case CellState::BLOCKED: std::cout
                << "# "; break;
                case CellState::SHALLOW: std::cout << "~ "; break;
                default:                 std::cout << "? "; break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    Board b(10, 10);

    std::cout << "=== Tabuleiro inicial ===\n";
    printBoard(b);

    // --- Posicionamento básico ---
    b.placeShip(2, 3, 3, true); // navio de tamanho 3 horizontal em (2,3)
    std::cout << "=== Apos posicionar navio ===\n";
    printBoard(b);

    // --- Tiros ---
    b.shoot(2, 3); // acerto
    b.shoot(5, 5); // erro
    b.shoot(2, 3); // repetido -> deve ser rejeitado (não muda nada)
    std::cout << "=== Apos tiros ===\n";
    printBoard(b);

    std::cout << "Tiro (10,0) valido? " << b.isValidShot(10, 0) << " (esperado: 0)\n";
    std::cout << "Tiro repetido (2,3) valido? " << b.isValidShot(2, 3) << " (esperado: 0)\n\n";

    // --- Testes de terreno (BLOCKED e SHALLOW) ---
    b.setCell(0, 0, CellState::BLOCKED);
    b.setCell(0, 1, CellState::SHALLOW);

    std::cout << "Navio tamanho 2 em (0,0) sobre BLOCKED? "
              << b.canPlace(0, 0, 2, true) << " (esperado: 0)\n";
    std::cout << "Navio tamanho 1 em (0,1) sobre SHALLOW? "
              << b.canPlace(0, 1, 1, true) << " (esperado: 1)\n";
    std::cout << "Navio tamanho 2 em (0,1) sobre SHALLOW? "
              << b.canPlace(0, 1, 2, true) << " (esperado: 0)\n\n";

    // Tiro em célula BLOCKED sem navio -> deve virar MISS
    b.shoot(0, 0);
    std::cout << "Estado apos atirar em BLOCKED vazio (esperado MISS): ";
    switch (b.getCell(0, 0)) {
        case CellState::MISS: std::cout << "MISS (correto)\n"; break;
        default: std::cout << "ERRO - nao virou MISS\n"; break;
    }

    // --- Colisão / vizinhança de Moore ---
    Board b2(10, 10);
    b2.placeShip(4, 4, 2, true); // navio horizontal ocupando (4,4)-(4,5)
    std::cout << "\nNavio adjacente (diagonal) em (3,3) tam.1 -> "
              << b2.canPlace(3, 3, 1, true) << " (esperado: 0, colisao Moore)\n";
    std::cout << "Navio distante em (8,8) tam.1 -> "
              << b2.canPlace(8, 8, 1, true) << " (esperado: 1)\n";

    // --- removeShip ---
    b2.removeShip(4, 4, 2, true);
    std::cout << "\nApos removeShip, (4,4) esta EMPTY? "
              << (b2.getCell(4, 4) == CellState::EMPTY) << " (esperado: 1)\n";

    // --- hasAnyShipRemaining ---
    Board b3(5, 5);
    std::cout << "\nTabuleiro vazio tem navio restante? "
              << b3.hasAnyShipRemaining() << " (esperado: 0)\n";
    b3.placeShip(0, 0, 2, true);
    std::cout << "Apos posicionar navio, tem navio restante? "
              << b3.hasAnyShipRemaining() << " (esperado: 1)\n";
    b3.shoot(0, 0);
    b3.shoot(0, 1);
    std::cout << "Apos afundar o unico navio, ainda tem navio restante? "
              << b3.hasAnyShipRemaining() << " (esperado: 0)\n";

    // --- reset ---
    b3.reset();
    std::cout << "\nApos reset, tabuleiro esta vazio? "
              << (!b3.hasAnyShipRemaining()) << " (esperado: 1)\n";

    return 0;
}