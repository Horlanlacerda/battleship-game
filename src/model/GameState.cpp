#include "GameState.h"


// --- boardSize ----------------------------------->

// Centraliza o tamanho de cada mapa num único lugar.
// Se o professor pedir para mudar, só muda aqui.
std::pair<int,int> GameState::boardSize(MapType map) {
    switch (map) {
        case MapType::ACUDE:  return {5,  5};
        case MapType::LAGO:   return {8,  8};
        case MapType::OCEANO: return {10, 10};
    }
    return {10, 10}; // fallback de segurança
}


// --- buildFrota ----------------------------------->

// Cria a lista de navios conforme as regras de cada mapa.
// Todos começam em (0,0) horizontal: o posicionamento real
// ocorre depois no PlacementScreen ou AutoPlacer.
std::vector<Ship> GameState::buildFleet(MapType map) {
    std::vector<Ship> fleet;


    // Fica MUITO DIFÍCIL POSICIONAR UM NAVIO GRANDE tendo que manter a regra dos dois livres
    if (map == MapType::ACUDE) {
        fleet.emplace_back(ShipType::SMALL,  0, 0, true);
        fleet.emplace_back(ShipType::SMALL, 0, 0, true);   // Trocou de MEDIUM para SMALL
        fleet.emplace_back(ShipType::MEDIUM,  0, 0, true); // Trocou de LARGE para MEDIUM
    }
    else if (map == MapType::LAGO) {
        fleet.emplace_back(ShipType::SMALL,  0, 0, true);
        fleet.emplace_back(ShipType::SMALL,  0, 0, true);
        fleet.emplace_back(ShipType::MEDIUM, 0, 0, true);
        fleet.emplace_back(ShipType::MEDIUM, 0, 0, true);
        fleet.emplace_back(ShipType::LARGE,  0, 0, true);
    }
    else { // OCEANO
        fleet.emplace_back(ShipType::SMALL,  0, 0, true);
        fleet.emplace_back(ShipType::SMALL,  0, 0, true);
        fleet.emplace_back(ShipType::SMALL,  0, 0, true);
        fleet.emplace_back(ShipType::MEDIUM, 0, 0, true);
        fleet.emplace_back(ShipType::MEDIUM, 0, 0, true);
        fleet.emplace_back(ShipType::LARGE,  0, 0, true);
        fleet.emplace_back(ShipType::LARGE,  0, 0, true);
    }

    return fleet;
}


// --- Construtor ----------------------------------->

// A lista de inicialização executa na ordem dos atributos declarados no .h:
//   1. mapType >> guarda qual mapa foi escolhido
//   2. playerBoard / computerBoard >> criados com o tamanho certo
//   3. playerShips / computerShips >> frotas montadas por buildFleet()
//   4. currentTurn — humano sempre começa
//
// O corpo do construtor (entre {}) aplica o terreno DEPOIS que os boards
// já foram criados — setCell() só funciona em board já inicializado.
GameState::GameState(MapType map)
    : mapType(map),
      playerBoard(boardSize(map).first,  boardSize(map).second),
      computerBoard(boardSize(map).first, boardSize(map).second),
      playerShips(buildFleet(map)),
      computerShips(buildFleet(map)),
      currentTurn(Turno::PLAYER)
{
    // Aplica terreno nos dois boards.
    // Feito no corpo (não na lista) porque MapLayout::apply()
    // é uma chamada de função, não uma inicialização de atributo.
    MapLayout::apply(playerBoard,   map);
    MapLayout::apply(computerBoard, map);
}


// --- Turno ----------------------------------->

Turno GameState::getCurrentTurn() const {return currentTurn;}

void GameState::switchTurn() {
    // Operador ternário: se PLAYER → vira COMPUTER, senão → vira PLAYER
    currentTurn = (currentTurn == Turno::PLAYER)
                  ? Turno::COMPUTER
                  : Turno::PLAYER;
}


// --- Boards ----------------------------------->

// O & é crucial, ele retorna referência ao objeto real dentro do GameState,
// não uma cópia. Qualquer modificação feita pelo chamador afeta o board real.
Board& GameState::getPlayerBoard()   { return playerBoard; }
Board& GameState::getComputerBoard() { return computerBoard; }


// --- Navios ----------------------------------->

std::vector<Ship>& GameState::getPlayerShips()   { return playerShips; }
std::vector<Ship>& GameState::getComputerShips() { return computerShips; }

MapType GameState::getMapType() const { return mapType; }


// --- Game State (literalmente) ----------------------------------->

// A lógica de fim de jogo vive aqui:
// se o computerBoard não tem mais navios >> jogador ganhou
// se o playerBoard não tem mais navios >> computador ganhou
GameResult GameState::checkResult() const {
    if (!computerBoard.hasAnyShipRemaining()) return GameResult::PLAYER_GANHOU;
    if (!playerBoard.hasAnyShipRemaining())   return GameResult::COMPUTER_GANHOU;
    return GameResult::OCORRENDO;
}

bool GameState::isGameOver() const {
    return checkResult() != GameResult::OCORRENDO;
}


// --- Timer ----------------------------------->

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