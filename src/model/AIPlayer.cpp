#include "model/AIPlayer.h"

#include <random>
#include <stdexcept>

AIPlayer::AIPlayer():mode(AIMode::HUNTING){}


std::pair<int, int> AIPlayer::makeShot(Board& playerBoard){

    switch (mode)
    {
    case AIMode::HUNTING:
        return huntingShot(playerBoard);
    
    case AIMode::TARGETING:
        // TODO: Implememtar estratégia de Targeting ainda.
        return huntingShot(playerBoard);
    }

    throw std::runtime_error("Modo de IA desconhecido.");
}


void AIPlayer::onShotResult(int row, int col, CellState result){

    if(result == CellState::HIT){
        mode = AIMode::TARGETING;

        // TODO: Adicionar células adjacentes ao vetor targets para que a IA possa explorá-las no modo Targeting.
    }
    else{

        // Enquanto o Targeting não estiver implementado, a IA permanece no modo Hunting.

    }
}


std::pair<int, int> AIPlayer::huntingShot(const Board& board){

    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<int> rowDistribution(
        0, board.getRows() - 1
    );

    std::uniform_int_distribution<int> colDistribution(
        0, board.getCols() - 1
    );

    int row; 
    int col;

    do {
        row = rowDistribution(generator);
        col = colDistribution(generator);

    } while(!notAttacked(board, row, col));
        
    return {row, col};
}

bool AIPlayer::notAttacked(const Board& board, int row, int col) const{

    CellState state = board.getCell(row, col);

    return state == CellState::EMPTY || state == CellState::SHIP;
}
