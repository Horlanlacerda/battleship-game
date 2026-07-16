#include "GameState.h"

GameState::GameState(int rows, int cols)
    : playerBoard(rows, cols),
      computerBoard(rows, cols),
      currentTurn(Turno::PLAYER)
{}

Turno GameState::getCurrentTurn() const { return currentTurn; }

void GameState::switchTurn() {
    currentTurn = (currentTurn == Turno::PLAYER)
                  ? Turno::COMPUTER
                  : Turno::PLAYER;
}

// Lembrando que o & é crucial: sem ele, retornaria uma CÓPIA do tabuleiro
// qualquer modificação externa afeta o tabuleiro REAL dentro do GameState
Board& GameState::getPlayerBoard()    { return playerBoard; }
Board& GameState::getComputerBoard()  { return computerBoard; }

// A lógica de fim de jogo vive aqui:
// se o computerBoard não tem mais navios >> jogador ganhou
// se o playerBoard não tem mais navios >> computador ganhou
GameResult GameState::checkResult() const {
    if (!computerBoard.hasAnyShipRemaining()) return GameResult::PLAYER_WIN;
    if (!playerBoard.hasAnyShipRemaining())   return GameResult::COMPUTER_WIN;
    return GameResult::ONGOING;
}

bool GameState::isGameOver() const {
    return checkResult() != GameResult::ONGOING;
}

void GameState::startTimer() {

    // Guarda o ponto de partida para cálculos futuros de tempo decorrido
    startTime = std::chrono::steady_clock::now();
}

int GameState::getElapsedSeconds() const {
    auto now = std::chrono::steady_clock::now(); // pega o instante atual

    // 1. Subtrai o instante inicial do atual
    //    'now - startTime' gera uma 'duration' (tempo decorrido)
    // 2. duration_cast<seconds> converte essa duração para segundos
    //    (descarta frações menores, como milissegundos)
    // 3. .count() extrai o valor numérico (quantos segundos)
    // 4. (int) converte para inteiro (trunca casas decimais)
    return (int)std::chrono::duration_cast<std::chrono::seconds>(
        now - startTime).count();
}