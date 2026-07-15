#include <iostream>
#include "../include/model/Ship.h"

void printOccupiedCells(const Ship& ship) {
    std::cout << "Celulas ocupadas: ";
    for (const auto& cell : ship.getOccupiedCells()) {
        std::cout << "(" << cell.first << "," << cell.second << ") ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "=== Teste manual da classe Ship ===" << std::endl << std::endl;

    // Cria um navio pequeno (tamanho 2), horizontal, comecando em (0,0)
    Ship smallShip(ShipType::SMALL, 0, 0, true);

    std::cout << "Tipo: " << smallShip.getTypeName() << std::endl;
    std::cout << "Tamanho: " << smallShip.getSize() << std::endl;
    std::cout << "Horizontal? " << (smallShip.isHorizontal() ? "Sim" : "Nao") << std::endl;
    printOccupiedCells(smallShip);

    std::cout << std::endl;

    // Testa occupiesCell
    std::cout << "Ocupa (0,0)? " << (smallShip.occupiesCell(0, 0) ? "Sim" : "Nao") << std::endl;
    std::cout << "Ocupa (0,1)? " << (smallShip.occupiesCell(0, 1) ? "Sim" : "Nao") << std::endl;
    std::cout << "Ocupa (5,5)? " << (smallShip.occupiesCell(5, 5) ? "Sim" : "Nao") << std::endl;

    std::cout << std::endl;

    // Simula tiros
    std::cout << "--- Simulando tiros ---" << std::endl;
    std::cout << "Afundado antes de qualquer tiro? " << (smallShip.isSunk() ? "Sim" : "Nao") << std::endl;

    smallShip.hit();
    std::cout << "Apos 1 tiro -> Segmentos restantes: " << smallShip.getRemainingSegments()
               << " | Afundado? " << (smallShip.isSunk() ? "Sim" : "Nao") << std::endl;

    smallShip.hit();
    std::cout << "Apos 2 tiros -> Segmentos restantes: " << smallShip.getRemainingSegments()
               << " | Afundado? " << (smallShip.isSunk() ? "Sim" : "Nao") << std::endl;

    // Testa hit extra (deve ser ignorado com seguranca)
    smallShip.hit();
    std::cout << "Apos 3 tiros (extra) -> Segmentos restantes: " << smallShip.getRemainingSegments()
               << " | Afundado? " << (smallShip.isSunk() ? "Sim" : "Nao") << std::endl;

    std::cout << std::endl;

    // Testa um navio vertical, so pra confirmar as celulas
    std::cout << "--- Navio grande, vertical, comecando em (2,3) ---" << std::endl;
    Ship largeShip(ShipType::LARGE, 2, 3, false);
    printOccupiedCells(largeShip);

    return 0;
}