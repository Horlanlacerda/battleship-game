#pragma once

#include "../model/Board.h"
#include "../model/Ship.h"
#include <vector>

/**
 * @brief Responsável pelo posicionamento automático dos navios.
 * 
 * @details A classe AutoPlacer realiza o posicionamento aleatório dos navios
 * no tabuleiro, sem necessidade de interação manual do jogador.
 * 
 * Para cada navio, são sorteadas uma posição inicial e uma orientação.
 * A posição é validada utilizando Board::canPlace(). Caso seja válida,
 * o navio é inserido no tabuleiro através de Board::placeShip().
 * 
 * O processo é repetido até que todos os navios sejam posicionados
 * ou o limite máximo de tentativas seja atingido.
 * 
 * @author Horlan Lacerda
 * @version 1.0
 * @date 22/07/2026
 */

class AutoPlacer {
    public:

    /**
     * @brief Posiciona automaticamente todos os navios no tabuleiro.
     * 
     * @details Limpa o tabuleiro antes de iniciar o posicionamento e,
     * para cada navio, tenta encontrar aleatoriamente uma posição válida.
     * A posição e a orientação são sorteadas até que o posicionamento
     * seja válido ou o limite máximo de tentativas seja atingido.
     * 
     * @param board Tabuleiro onde os navios serão posicionados.
     * @param ships Vetor contendo os navios que deverão ser posicionados.
     * 
     * @return true se todos os navios forem posicionados com sucesso;
     * @return false se algum navio não puder ser posicionado dentro
     * do limite máximo de tentativas;
     */
    static bool place(Board& board, std::vector<Ship>& ships);
};