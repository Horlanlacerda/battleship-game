#pragma once

#include "../model/Board.h"
#include "../model/Ship.h"
#include <vector>

class AutoPlacer {
    public:
    static bool place(Board& board, std::vector<Ship>& ships);
};