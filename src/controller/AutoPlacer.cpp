#include "AutoPlacer.h"

#include <algorithm>
#include <cstddef>
#include <random>

/**
 * @brief Implementação do posicionamento automático dos navios.
 * 
 * @details A classe AutoPlacer é responsável por realizar o posicionamento
 * automático dos navios no tabuleiro. Para cada navio, uma posição e uma
 * orientação são escolhidas aleatoriamente e validadas por meio das regras
 * definidas pela classe Board.
 * 
 * O posicionamento é realizado somente quando a posição sorteada é válida,
 * respeitando os limites do tabuleiro, as restrições de terreno e a distância
 * mínima entre os navios.
 * 
 * Caso não seja possível posicionar um navio após um número máximo de tentativas,
 * o processo é interrompido e o método retorna false.
 * 
 * @author Horlan Lacerda
 * @since 1.0
 * @date 22/07/2026
 */

 /**
  * @brief Realiza o posicionamento automático dos navios.
  * 
  * @details O método limpa o tabuleiro e tenta posicionar cada navio em uma
  * posição aleatória, escolhendo também aleatoriamente entre as orientações
  * horizontal e vertical.
  * 
  * Antes de realizar o posicionamento, a posição sorteada é validada pelo
  * método Board::canPlace(). Quando a posição é válida, o navio é registrado
  * no tabuleiro por meio de Board::placeShip().
  * 
  * Cada navio possui um limite máximo de tentativas para evitar que o programa
  * entre em um loop infinito caso não exista uma posição válida disponível.
  * 
  * @param board Tabuleiro onde os navios serão posicionados.
  * @param ships Vetor contendo os navios que deverão ser posicionados.
  * 
  * @return true se todos os navios forem posicionados com sucesso.
  * @return false se algum navio não puder ser posicionado após o número mácimo de tentativas.
  */

bool AutoPlacer::place(Board& board, std::vector<Ship>& ships){

    // Gerador de números aleatórios
    std::random_device rd;
    std::mt19937 generator(rd());

    // Tenta posicionar os navios do maior para o menor: o navio Grande
    // (5 células) só cabe numa linha/coluna totalmente livre, entao ele
    // precisa ser posicionado primeiro, enquanto o tabuleiro ainda esta
    // vazio. Isso so define a ORDEM das tentativas — a lista "ships"
    // (e portanto o indice de cada navio) nao e alterada.
    std::vector<std::size_t> order(ships.size());
    for (std::size_t i = 0; i < order.size(); i++) order[i] = i;
    std::sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b) {
        return ships[a].getSize() > ships[b].getSize();
    });

    // Se algum navio nao couber, a frota inteira e re-sorteada do zero
    // (em vez de so desistir daquele navio) — do contrario a partida
    // seguia com um navio a menos no tabuleiro, e o jogador vencia cedo
    // demais ao afundar so os navios que de fato foram colocados.
    const int maxFleetAttempts = 200;

    for (int fleetAttempt = 0; fleetAttempt < maxFleetAttempts; fleetAttempt++) {
        board.reset();
        bool allPlaced = true;

        for (std::size_t idx : order) {
            Ship& ship = ships[idx];
            bool placed = false;

            // Limite de segurança para evitar loop infinito caso
            // não exista mais nenhuma posição válida no tabuleiro.
            const int maxAttempts = 1000;

            for(int attempt = 0; attempt < maxAttempts && !placed; attempt++){

                // Sorteia a orientação:
                // true  = horizontal
                // false = vertical
                std::uniform_int_distribution<int> orientacionDistribution(0, 1);
                bool horizontal = orientacionDistribution(generator) == 1;

                 // Sorteia uma posição dentro dos limites do tabuleiro.
                std::uniform_int_distribution<int> rowDistribution(0, board.getRows() -1);

                std::uniform_int_distribution<int> colDistribution(0, board.getCols() -1);

                int row = rowDistribution(generator);
                int col = colDistribution(generator);

                // Verifica se o navio pode ser colocado nessa posição.
                if(board.canPlace(row, col, ship.getSize(), horizontal)){

                    board.placeShip(row, col, ship.getSize(), horizontal);

                    ship.setPosition(row, col, horizontal);

                    placed = true;
                }
            }

            if (!placed) {
                allPlaced = false;
                break;
            }
        }

        // Toda a frota coube: sucesso.
        if (allPlaced) return true;
    }

    // Praticamente impossível de chegar aqui (200 tentativas completas),
    // mas se acontecer, o chamador precisa saber que a frota ficou incompleta.
    return false;
}