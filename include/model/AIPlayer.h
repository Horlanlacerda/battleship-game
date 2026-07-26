#pragma once

#include "Board.h"
#include <vector>
#include <utility>

enum class AIMode{
    HUNTING,
    TARGETING
};

class AIPlayer{

    public:

        AIPlayer();

        std::pair<int, int> makeShot(Board& playerBoard);

        void onShotResult(int row, int col, CellState result);
    
    private:

        AIMode mode;

        std::vector<std::pair<int, int>> targets;

        std::pair<int, int> huntingShot(const Board& board);

        bool notAttacked(const Board& board, int row, int col) const; // O const no final significa que a função não modifica o objeto a qual ele pertence.

}