#pragma once

#include "Board.h"
#include <vector>
#include <utility>

/**
 * @brief Representa o jogador controlado pela inteligência artificial.
 * 
 * @details A classe AIPlayer é responsável por controlar as ações da
 * inteligência artificial durante a partida de Batalha Naval.
 * 
 * A IA possui diferentes modos de comportamento. No modo Hunting (Caça), 
 * a inteligência artificial procura por navios inimigos realizando disparos
 * em células ainda não atacadas do tabuleiro.
 * 
 * O modo Targeting (Alvo) é reservado para uma implementação posterior,
 * sendo utilizado para permitir que a IA explore regiões próximas após
 * encontrar um navio adversário.
 * 
 * @author Horlan Lacerda
 * @version 1.0
 * @date 26/07/2026
 */

enum class AIMode{

    /// Modo de caça: procura por navios em células ainda não atacadas.
    HUNTING,

    /// Modo de alvo: concentra os disparos em regiões próximas a um acerto.
    TARGETING
};

class AIPlayer{

    public:

        /**
         * @brief Construtor padrão da inteligência artifical.
         * 
         * @details Inicializa a IA em seu modo inicial de operação e prepara
         * as estruturas necessárias para o controle dos disparos.
         */
        AIPlayer();


        /**
         * @brief Realiza um disparo contro o tabuleiro do jogador.
         * 
         * @details Seleciona uma célula ainda não atacada de acordo com o
         * modo atual de operação da inteligência artificial e realiza o disparo.
         * 
         * @param playerBoard Referência para o tabuleiro do jogador adversário.
         * 
         * @return Um par contendo a linha e a coluna da célula escolhida
         * para o disparo.
         */
        std::pair<int, int> makeShot(Board& playerBoard);


        /**
         * @brief Informa à IA o resultado do último disparo realizado.
         * 
         * @details Permite que a inteligência artificial atualize seu estado
         * interno de acordo com o resultado obtido pelo disparo.
         * 
         * @param row Linha da célula atingida pelo disparo.
         * @param col Coluna da célula atingida pelo disparo.
         * @param result Estado resultante da célula após o disparo.
         */
        void onShotResult(int row, int col, CellState result);
    

    private:

        /// Modo atual de operação da inteligência artificial.
        AIMode mode;


        /**
         * @brief Lista de células que serão utilizadas como alvos.
         * 
         * @details A estrutura é utilizada para armazenar possíveis células
         * de interesse para o modo Targeting, permitindo que a IA explore
         * posições próximas após acertar um navio.
         */
        std::vector<std::pair<int, int>> targets;


        /**
         * @brief Realiza um disparo utilizando a estratégia de Hunting.
         * 
         * @details Seleciona uma célula ainda não atacada do tabuleiro para
         * realizar uma tentativa de encontrar um navio adversário.
         * 
         * @param board Tabuleiro do jogador adversário.
         * 
         * @return Um par contendo a linha e a coluna escolhidas para o disparo.
         */
        std::pair<int, int> huntingShot(const Board& board);


        /**
         * @brief Verifica se uma célula ainda não foi atacada.
         * 
         * @details Analisa o estado da célula indicada e determina se ela
         * ainda pode ser escolhida pela inteligência artificial para realizar
         * um novo disparo.
         * 
         * @param board Tabuleiro que será consultado.
         * @param row Linha da célula a ser verificada.
         * @param col Coluna da célula a ser verificada.
         * 
         * @return true se a célula não foi atacada;
         * @return false se a célula já tenha sido atacada.
         */
        bool notAttacked(const Board& board, int row, int col) const; // O const no final significa que a função não modifica o objeto a qual ele pertence.

};