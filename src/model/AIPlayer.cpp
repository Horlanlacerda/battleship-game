#include "model/AIPlayer.h"

#include <random>
#include <stdexcept>

/**
 * @brief Implementação da inteligência artificial do Jogo Batalha Naval.
 * 
 * @details Contém a implementação dos métodos responsáveis pelo controle
 * do comportamento da IA durante a partida.
 * 
 * A inteligência artificial opera em dois modos distintos: 
 * 
 * A inteligência artificial opera em dois modos distintos:
 * - Hunting (Caça): realiza disparos aleatórios em células ainda não atacadas.
 * - Targeting (Alvo): após acertar um navio, passa a explorar as células
 *   adjacentes ao ponto do acerto até que o navio seja afundado.
 * 
 * Também são implementados as transições entre os modos de operação,
 * bem como o gerenciamento dos alvos candidatos durante a estratégia
 * de Targeting.
 * 
 * @author Horlan Lacerda
 * @version 1.0
 * @date 04/08/2026
 * 
 */


/**
 * @brief Construtor da inteligência artificial.
 *
 * @details Inicializa a IA no modo Hunting (Caça), estado inicial da
 * partida, no qual são realizados disparos aleatórios em posições ainda
 * não atacadas do tabuleiro adversário.
 */
AIPlayer::AIPlayer():mode(AIMode::HUNTING){}


/**
 * @brief Determina a próxima posição de disparo da IA.
 * 
 * @details Seleciona a posição do próximo disparo de acordo com o modo
 * atual de operação da inteligência artificial.
 * 
 * No modo Hunting, um disparo aleatório é escolhido dentre as células
 * ainda não atacadas.
 * 
 * No modo Targeting, a IA utiliza as posições armazenadas na lista de
 * alvos candidatos, priorizando células adjacentes a um acerto anterior.
 * Caso não existam mais posições válidas, a IA retorna automaticamente
 * ao modo Hunting.
 * 
 * @param playerBoard Referência para o tabuleiro do jogador adversário.
 * 
 * @return Um par contendo a linha a coluna da próxima posição de disparo.
 * 
 * @throw std::runtime_error Caso o modo atual da IA seja inválido.
 */
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


/**
 * @brief Processa o resultado de um disparo realizado pela IA.
 * 
 * @details Quando um disparo resulta em um acerto (HIT), a inteligência
 * artificial altera seu modo de operação para Targeting e adiciona as
 * quatro células adjacentes ao ponto atingido como possíveis alvos para
 * os próximos disparos.
 * 
 * Caso o resultado não represente um acerto, nenhuma alteração é
 * realizada no comportamento atual da IA.
 * 
 * @param row Linha da célula atingida.
 * @param col Coluna da célula atingida.
 * @param result Resultado obtido após o disparo.
 */
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


/**
 * @brief Processa o afundamento de um navio adversário.
 *
 * @details Remove todos os alvos candidatos armazenados durante o modo
 * Targeting, uma vez que o navio correspondente já foi completamente
 * destruído.
 *
 * Após a limpeza da lista de alvos, a inteligência artificial retorna
 * ao modo Hunting para reiniciar a busca por novos navios inimigos.
 */
void AIPlayer::onShipSunk(){

    // O navio foi afundado, portanto os alvos relacionados
    // ao navio atual não são mais necessários.
    targets.clear();

    // Retorna ao modo Hunting para procurar outro navio.
    mode = AIMode::HUNTING;
}


/**
 * @brief Seleciona um disparo aleatório no modo Hunting.
 * 
 * @details Gera coordenadas aleatórias dentro dos limites do tabuleiro
 * até encontrar uma célula que ainda não tenha sido atacada.
 * 
 * O método utiliza um gerador de números pseudoaleatórios baseado na
 * biblioteca padrão da linguagem.
 * 
 * @param board Referência para o tabuleiro do adversário.
 * 
 * @return Um par contendo a linha e a coluna selecionadas para o disparo.
 */
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


/**
 * @brief Verifica se um determinada posição ainda não foi atacada.
 * 
 * @details Inicialmente verifica se as coordenadas informadas pertencem
 * aos limites do tabuleiro. Caso estejam fora da área válida, a posição
 * é considerada indisponível para disparo.
 * 
 * Quando a posição é válida, consulta o estado da célula e verifica se
 * ela ainda pode ser utilizada como alvo pela inteligência artificial.
 * 
 * @param board Referência para o tabuleiro consultado.
 * @param row Linha da célula.
 * @param col Coluna da célula.
 *
 * @return true Caso a célula ainda não tenha sido atacada.
 * @return false Caso a célula esteja fora dos limites ou já tenha sido atacada.
 */
bool AIPlayer::notAttacked(const Board& board, int row, int col) const{

    // Verifica se a posição está dentro dos limites do tabuleiro.
    if(row < 0 || row >= board.getRows() || col < 0 || col >= board.getCols()){
        return false;
    }

    CellState state = board.getCell(row, col);

    return state == CellState::EMPTY || state == CellState::SHIP;
}
