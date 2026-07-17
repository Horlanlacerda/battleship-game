
#include "Board.h"
#include <stdexcept> // biblioteca padrão para exceções, usado para std::out_of_range do


// tenho que usar o Board:: pra especificar que é uma função/método da minha classe

// --- Construtor ------------------------------------------------------------>
// Inicializa a matriz rows × cols com todas as células EMPTY
Board::Board(int rows, int cols)
    : rows(rows), cols(cols),
      grid(rows, std::vector<CellState>(cols, CellState::EMPTY))
{}

// --- Ajudante interno ------------------------------------------------------------>
// Verifica se (row, col) está dentro dos limites da matriz
bool Board::inBounds(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

// --- canPlace ------------------------------------------------------------>
// Retorna true se um navio de 'size' células cabe na posição sem conflito
// NÃO modifica nada, só consulta
bool Board::canPlace(int row, int col, int size, bool horizontal) const {

    for (int i = 0; i < size; i++) {
        int shipRow = horizontal ? row : row + i;
        int shipCol = horizontal ? col + i : col;

        // 1. célula dentro do tabuleiro?
        if (!inBounds(shipRow, shipCol))
            return false;

        // 2. Restrição de terreno (NOVO)
        // BLOCKED: nenhum navio pode entrar.
        if (grid[shipRow][shipCol] == CellState::BLOCKED)
            return false;

        // SHALLOW: só navios pequenos (size == 1) podem entrar
        // Se o navio tem 2 ou mais células, rejeita qualquer célula rasa
        if (grid[shipRow][shipCol] == CellState::SHALLOW && size > 1)
            return false;

        // 3. Colisão com navios existentes
        // Verifica a própria célula e todas as 8 vizinhas (aparentemente isso se chama 'vizinhança de Moore')
        for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
            for (int colOffset = -1; colOffset <= 1; colOffset++) {
                int neighborRow = shipRow + rowOffset;
                int neighborCol = shipCol + colOffset;
                // Pula coordenadas fora do tabuleiro
                if (!inBounds(neighborRow, neighborCol)) continue;
                // Se qualquer vizinha (ou a própria célula) for SHIP, rejeita
                if (grid[neighborRow][neighborCol] == CellState::SHIP)
                    return false;
            }
        }
    }

    return true; // todas as células passaram nas três regras
}


// --- placeShip ------------------------------------------------------------>
// Coloca o navio na matriz. Retorna false se canPlace falhar.
bool Board::placeShip(int row, int col, int size, bool horizontal) {
    if (!canPlace(row, col, size, horizontal))
        return false;

    for (int i = 0; i < size; i++) {
        int shipRow = horizontal ? row     : row + i;
        int shipCol = horizontal ? col + i : col;
        grid[shipRow][shipCol] = CellState::SHIP;
    }
    return true;
}

// --- removeShip ------------------------------------------------------------>
// Desfaz o posicionamento, é útil quando o jogador quer reposicionar.
void Board::removeShip(int row, int col, int size, bool horizontal) {
    for (int i = 0; i < size; i++) {
        int shipRow = horizontal ? row     : row + i;
        int shipCol = horizontal ? col + i : col;
        if (inBounds(shipRow, shipCol) && grid[shipRow][shipCol] == CellState::SHIP)
            grid[shipRow][shipCol] = CellState::EMPTY;
    }
}


// --- isValidShot ------------------------------------------------------------>
// Um tiro é válido se: está dentro dos limites E a célula ainda não foi
// atacada (não é HIT nem MISS).
bool Board::isValidShot(int row, int col) const {
    if (!inBounds(row, col))
        return false;

    CellState s = grid[row][col];

    // Pode atirar em: EMPTY, SHIP, BLOCKED, SHALLOW
    // Não pode atirar em: HIT, MISS (já atacadas)
    return s != CellState::HIT && s != CellState::MISS;
}


// --- shoot ------------------------------------------------------------>
// Registra o tiro e retorna o novo estado da célula.
// O Controller usa esse retorno para saber o que mostrar na tela.
//  Tiro em BLOCKED ou SHALLOW sem navio → MISS (água), O jogador "desperdiçou" o tiro em terreno, mas a jogada é válida.

CellState Board::shoot(int row, int col) {
    // Tiro inválido: retorna estado atual sem modificar nada
    if (!isValidShot(row, col))
        return grid[row][col];

    if (grid[row][col] == CellState::SHIP) {
        // Acerto: navio atingido
        grid[row][col] = CellState::HIT;
    } else {
        // Erro: água, terreno bloqueado sem navio, ou raso sem navio
        // Todos viram MISS para marcar visualmente que já foi atacado
        grid[row][col] = CellState::MISS;
    }

    return grid[row][col];
}


// --- Getters ------------------------------------------------------------>
CellState Board::getCell(int row, int col) const {
    if (!inBounds(row, col))
        throw std::out_of_range("Board::getCell >> coordenada fora dos limites");
    return grid[row][col];
}

int Board::getRows() const { return rows; }
int Board::getCols() const { return cols; }


// --- Setters ------------------------------------------------------------>
// Escrita direta na matriz sem validação de regras de jogo
// Usado exclusivamente pelo MapLayout antes da fase de posicionamento
void Board::setCell(int row, int col, CellState state) {
    if (inBounds(row, col))
        grid[row][col] = state;
    // Coordenada fora dos limites é silenciosamente ignorada —
    // evita crash por configuração errada no MapLayout.
}


// --- hasAnyShipRemaining ------------------------------------------------------------>
// Percorre toda a matriz procurando alguma célula SHIP intacta.
// Se não encontrar, todos os navios foram afundados → jogo acabou.
bool Board::hasAnyShipRemaining() const {
    for (int rowAux = 0; rowAux < rows; rowAux++)
        for (int colAux = 0; colAux < cols; colAux++)
            if (grid[rowAux][colAux] == CellState::SHIP)
                return true;
    return false;
}

// --- reset ------------------------------------------------------------>
// Volta todo o tabuleiro para EMPTY
// usado para reiniciar partida.
void Board::reset() {
    for (auto& row : grid) // auto no lugar do std::vector<CellState> pra o compilador achar sozinho

    /*
     * o & é uma reference (e não ponteiro) ele é basicamente um apelido para a variável original ,
     * modifico ele >> modifico a variável original
     * se eu não usasse, o programa faria uma cópia pra cada cell no meu tabuleiro
     */
        for (auto& cell : row)

            cell = CellState::EMPTY;
    // Terrenos definidos pelo MapLayout também são apagados
    // Se reiniciar a partida mantendo o mapa, reaplique MapLayout::apply()
}