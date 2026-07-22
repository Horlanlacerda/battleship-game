#include "AutoPlacer.h"

#include <random>

bool AutoPlacer::place(Board& board, std::vector<Ship>& ships){

    std::random_device rd;
    std::mt19937 generator(rd());

    board.reset();

    for(Ship& ship : ships){

        bool placed = false;

        const int maxAttempts = 1000;

        for(int attempt = 0; attempt < maxAttempts && !placed; attempt++){

            std::uniform_int_distribution<int> orientacionDistribution(0, 1);
            bool horizontal = orientacionDistribution(generator) == 1;

            std::uniform_int_distribution<int> rowDistribution(0, board.getRows() -1);

            std::uniform_int_distribution<int> colDistribution(0, board.getCols() -1);

            int row = rowDistribution(generator);
            int col = colDistribution(generator);

            if(board.canPlace(row, col, ship.getSize(), horizontal)){
                board.placeShip(row, col, ship.getSize(), horizontal);

                placed = true;
            }
        }

        if(!placed){
            return false;
        }
    }

    return true;
}