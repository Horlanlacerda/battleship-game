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
        
        // Enquanro houver posições candidatas, tenta utilizá-las
        while(!targets.empty()){

            auto target = targets.back();
            targets.pop_back();

            int row = target.first;
            int col = target.second;

            // Verifica se a posição ainda está disponível para disparo.
            if(notAttacked(playerBoard, row, col)){
                return {row, col};
            }

        }

        // Caso não existam mais alvos válidos,
        // retorna ao modo Hunting.
        mode = AIMode::HUNTING;
        return huntingShot(playerBoard);
    }

    throw std::runtime_error("Modo de IA desconhecido.");
}


void AIPlayer::onShotResult(int row, int col, CellState result){

    if(result == CellState::HIT){

        mode = AIMode::TARGETING;

        // Adiciona as quatro células adjacentes ao ponto atingido.
        targets.push_back({row - 1, col}); // Cima
        targets.push_back({row + 1, col}); // Baixo
        targets.push_back({row, col - 1}); // Esquerda
        targets.push_back({row, col + 1}); // Direita
    }
}


void AIPlayer::onShipSunk(){

    // O navio foi afundado, portanto os alvos relacionados
    // ao navio atual não são mais necessários.
    targets.clear();

    // Retorna ao modo Hunting para procurar outro navio.
    mode = AIMode::HUNTING;
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

    // Verifica se a posição está dentro dos limites do tabuleiro.
    if(row < 0 || row >= board.getRows() || col < 0 || col >= board.getCols()){
        return false;
    }

    CellState state = board.getCell(row, col);

    return state == CellState::EMPTY || state == CellState::SHIP;
}
