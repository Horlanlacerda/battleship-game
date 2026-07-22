#pragma once
#include "Board.h"
#include <chrono>

/**
 * @brief Representa de quem é a vez de jogar
 * @author Georis
 * @date 16/07/2026
 */
enum class Turno {
    PLAYER,   ///< Vez do 👮 atirar
    COMPUTER  ///< Vez do 💻 atirar
};


/**
 * @brief Representa o estado atual da partida
 * @details Indica se o jogo está em andamento ou se já houve
 * um vencedor.
 * @author Georis
 * @date 16/07/2026
 */
enum class GameResult { 
    OCORRENDO,       ///< Partida em andamento, nenhum vencedor ainda.
    PLAYER_GANHOU,    ///< Jogador humano venceu (todos os navios do computador foram destruídos).
    COMPUTER_GANHOU   ///< Computador venceu (todos os navios do jogador foram destruídos).
};


/**
 * @brief Gerencia o estado global da partida de Batalha Naval
 * @details A classe GameState é o CONTROLLER do jogo, responsável por:
 * - Manter os tabuleiros do jogador e do computador
 * - Controlar a alternância de turnos (Player ↔ Computer)
 * - Verificar condições de vitória/derrota
 * - Gerenciar o tempo de duração da partida
 *
 * 
 * @author Georis
 * @date 16/07/2026
 */
class GameState {
public:
    /**
     * @brief Constrói o estado inicial de uma nova partida
     * @details Cria dois tabuleiros (jogador e computador) com as mesmas
     * dimensões e inicia o primeiro turno como Turn::PLAYER (ou seja, humano começa sempre).
     * O timer da partida NÃO é iniciado automaticamente, usamos startTimer()
     * quando quiser começar a contagem.
     * 
     * @param rows Número de linhas dos tabuleiros (vertical).
     * @param cols Número de colunas dos tabuleiros (horizontal).
     * 
     * @warning Valores inválidos para rows/cols (≤ 0) serão propagados
     * para o construtor de Board o que lançará excessão.
     * 
     * @see Board::Board(int, int)
     * @see startTimer() para iniciar a contagem de tempo.
     * 
     * @author Georis
     * @date 16/07/2026
     */
    GameState(int rows, int cols);  // cria os dois boards com o mesmo tamanho



    // --- Turno ----------------------------------->

    /**
     * @brief Retorna de quem é a vez atual no jogo
     * @return Turn::PLAYER se for a vez do jogador humano;
     *         Turn::COMPUTER se for a vez do computador.
     * 
     * @author Georis
     * @date 16/07/2026
     */
    Turno getCurrentTurn() const;


    /**
     * @brief Alterna o turno entre jogador e computador
     * @details Se o turno atual for PLAYER, passa para COMPUTER e vice-versa.
     * Deve ser chamada após cada jogada válida.
     * 
     * @author Georis
     * @date 16/07/2026
     */
    void switchTurn();



    // --- Boards ----------------------------------->

    /**
     * @brief Retorna uma referência ao tabuleiro do jogador
     * @details Permite acesso direto ao board do jogador para consulta
     * e modificação (posicionamento de navios, registro de tiros).
     * 
     * @return Board& Referência modificável do tabuleiro do jogador.
     * 
     * @warning A referência retornada permite modificar o tabuleiro
     * diretamente. Use com responsabilidade para não burlar as regras
     * do jogo (ex: alterar células diretamente sem passar por shoot()).
     * 
     * @author Georis
     * @date 16/07/2026
     */
    Board& getPlayerBoard();


    /**
     * @brief Retorna uma referência ao tabuleiro do computador
     * @details Permite acesso direto ao board do computador para consulta
     * e modificação (registro de tiros do jogador, posicionamento automático).
     * 
     * @return Board& Referência modificável do tabuleiro do computador.
     * 
     * @warning Evite modificar o tabuleiro do computador manualmente
     * durante o turno do jogador, pois isso pode comprometer a integridade
     * do jogo.
     * 
     * @author Georis
     * @date 16/07/2026
     */
    Board& getComputerBoard();



    // --- Estado da partida --------------------------->

    /**
     * @brief Verifica o resultado atual da partida
     * @details A lógica de vitória/derrota é:
     * > Se o computerBoard não tem mais navios >> PLAYER_GANHOU
     * > Se o playerBoard não tem mais navios >> COMPUTER_GANHOU
     * > Caso contrário >> OCORRENDO (jogo continua)
     * 
     * @return GameResult::PLAYER_GANHOU se todos os navios do computador foram destruídos;
     *         GameResult::COMPUTER_GANHOU se todos os navios do jogador foram destruídos;
     *         GameResult::OCORRENDO se ambos ainda têm navios intactos.
     * 
     * @see Board::hasAnyShipRemaining()
     * @see isGameOver()
     * 
     * @author Georis
     * @date 16/07/2026
     */
    GameResult checkResult() const;



    /**
     * @brief Verifica se a partida já terminou
     * @details Método auxiliar que retorna true se checkResult()
     * for diferente de GameResult::OCORRENDO.
     * 
     * @return true se o jogo terminou;
     *         false se o jogo ainda está em andamento.
     * 
     * @see checkResult() para obter quem venceu.
     * 
     * @author Georis
     * @date 16/07/2026
     */
    bool isGameOver() const;



    // --- Timer ----------------------------------->

    /**
     * @brief Inicia a contagem de tempo da partida
     * @details Registra o instante atual como ponto de partida.
     * Deve ser chamado quando o jogo começa efetivamente.
     * 
     * @warning Se chamado múltiplas vezes, reinicia a contagem
     * a partir da última chamada.
     * 
     * @see getElapsedSeconds()
     * 
     * @author Georis
     * @date 16/07/2026
     */
    void startTimer();


    /**
     * @brief Retorna o tempo decorrido desde o início da partida
     * @details Calcula a diferença entre o instante atual e o momento
     * em que startTimer() foi chamado.
     * 
     * @return Quantidade de segundos decorridos (valor inteiro, truncado).
     * 
     * @warning Se startTimer() nunca foi chamado, o time_point tem valor
     * indeterminado e o resultado será imprevisível.
     * 
     * @see startTimer()
     * 
     * @author Georis
     * @date 16/07/2026
     */
    int getElapsedSeconds() const;


private:
    Board playerBoard;
    Board computerBoard;
    Turno  currentTurn;    ///< Indica de quem é a vez atual (PLAYER ou COMPUTER).

    /**
     * @brief Momento em que o timer da partida foi iniciado
     * @details Utilizado para calcular o tempo total de jogo.
     * Inicializado por startTimer().
     */
    std::chrono::steady_clock::time_point startTime;
};