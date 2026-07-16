
#include "Board.h"
#include <stdexcept> // biblioteca padrão para exceções, usado para std::out_of_range do


// tenho que usar o Board:: pra especificar que é uma função/método da minha classe

// --- Construtor ------------------------------------------------------------>
// Inicializa a matriz rows × cols com todas as células EMPTY
Board::Board(int rows, int cols)
    : rows(rows), cols(cols),
      grid(rows, std::vector<CellState>(cols, CellState::EMPTY))
{}

// --- AJudante interno ------------------------------------------------------------>
// Verifica se (row, col) está dentro dos limites da matriz
bool Board::inBounds(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

// --- canPlace ------------------------------------------------------------>
// Retorna true se um navio de 'size' células cabe na posição sem conflito.
// NÃO modifica nada, só consulta.
bool Board::canPlace(int row, int col, int size, bool horizontal) const {

    for (int i = 0; i < size; i++) {
        int rowAux = horizontal ? row : row + i;
        int colAux = horizontal ? col + i : col;

        // 1. Célula dentro do tabuleiro?
        if (!inBounds(rowAux, colAux))
            return false;

        // 2. Célula já ocupada?
        if (grid[rowAux][colAux] == CellState::SHIP)
            return false;

        // 3. Alguma célula ADJACENTE (incluindo diagonais) é SHIP?
        // Navios não podem se encostar.
        for (int drow = -1; drow <= 1; drow++) {
            for (int dcol = -1; dcol <= 1; dcol++) {
                if (drow == 0 && dcol == 0) continue; // a própria célula, pular
                int nrow = rowAux + drow;
                int ncol = colAux + dcol;
                if (inBounds(nrow, ncol) && grid[nrow][ncol] == CellState::SHIP)
                    return false;
            }
        }
    }
    return true;
}


// --- placeShip ------------------------------------------------------------>
// Coloca o navio na matriz. Retorna false se canPlace falhar.
bool Board::placeShip(int row, int col, int size, bool horizontal) {
    if (!canPlace(row, col, size, horizontal))
        return false;

    for (int i = 0; i < size; i++) {
        int rowAux = horizontal ? row     : row + i;
        int colAux = horizontal ? col + i : col;
        grid[rowAux][colAux] = CellState::SHIP;
    }
    return true;
}

// --- removeShip ------------------------------------------------------------>
// Desfaz o posicionamento, é útil quando o jogador quer reposicionar.
void Board::removeShip(int row, int col, int size, bool horizontal) {
    for (int i = 0; i < size; i++) {
        int rowAux = horizontal ? row     : row + i;
        int colAux = horizontal ? col + i : col;
        if (inBounds(rowAux, colAux) && grid[rowAux][colAux] == CellState::SHIP)
            grid[rowAux][colAux] = CellState::EMPTY;
    }
}


// --- isValidShot ------------------------------------------------------------>
// Um tiro é válido se: está dentro dos limites E a célula ainda não foi
// atacada (não é HIT nem MISS).
bool Board::isValidShot(int row, int col) const {
    if (!inBounds(row, col))
        return false;
    CellState s = grid[row][col];
    // Só pode atirar em células ainda intactas
    return s == CellState::EMPTY || s == CellState::SHIP;
}


// --- shoot ------------------------------------------------------------>
// Registra o tiro e retorna o novo estado da célula.
// O Controller usa esse retorno para saber o que mostrar na tela.
CellState Board::shoot(int row, int col) {
    if (!isValidShot(row, col))
        return grid[row][col]; // devolve o estado atual sem mudar nada
    // (célula já atacada ou fora dos limites)

    if (grid[row][col] == CellState::SHIP) {
        grid[row][col] = CellState::HIT;
    } else {
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
     * modifico ele -> modifico a variável orginal
     * se eu não usasse , o programa faria uma cópia pra cada cell no meu tabuleiro
     */
        for (auto& cell : row)

            cell = CellState::EMPTY;
}