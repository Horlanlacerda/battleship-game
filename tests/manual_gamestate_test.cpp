#include <iostream>
#include "model/GameState.h"

int main() {
    // Testa os 3 mapas
    GameState acude(MapType::ACUDE);
    GameState lago(MapType::LAGO);
    GameState oceano(MapType::OCEANO);

    // Verifica tamanhos
    std::cout << "Acude: "  << acude.getPlayerBoard().getRows()
              << "x" << acude.getPlayerBoard().getCols()  << "\n"; // 5x5
    std::cout << "Lago: "   << lago.getPlayerBoard().getRows()
              << "x" << lago.getPlayerBoard().getCols()   << "\n"; // 8x8
    std::cout << "Oceano: " << oceano.getPlayerBoard().getRows()
              << "x" << oceano.getPlayerBoard().getCols() << "\n"; // 10x10

    // Verifica frotas
    std::cout << "Frota Acude: "  << acude.getPlayerShips().size()
              << " navios\n";  // esperado: 3
    std::cout << "Frota Lago: "   << lago.getPlayerShips().size()
              << " navios\n";  // esperado: 5
    std::cout << "Frota Oceano: " << oceano.getPlayerShips().size()
              << " navios\n";  // esperado: 7

    // Verifica terreno do Acude (ilha no centro: linha 2, col 2)
    CellState centro = acude.getPlayerBoard().getCell(2, 2);
    std::cout << "Celula (2,2) Acude: "
              << (centro == CellState::BLOCKED ? "BLOCKED (saída esperada)" : "ERRADO")
              << "\n";

    // Verifica fim de jogo (nenhum navio posicionado ainda = false)
    std::cout << "Jogo terminado? "
              << (oceano.isGameOver() ? "Sim" : "Não")
              << "\n";

    return 0;
}//
// Created by georis on 25/07/2026.
//
