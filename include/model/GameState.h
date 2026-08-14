#pragma once
#include "Board.h"
#include "Ship.h"
#include "MapLayout.h"  // MapType vem daqui, aoinvés de setar tudo manualmente
#include <vector>
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
 * @details A classe GameState é o quase como um Controller do jogo, responsável por:
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
     * @brief Constrói o estado inicial de uma partida para o mapa escolhido
     *
     * @details O construtor executa automaticamente, nesta ordem:
     * 1. Determina as dimensões do tabuleiro para o MapType recebido
     * 2. Cria playerBoard e computerBoard com essas dimensões
     * 3. Aplica o terreno (BLOCKED/SHALLOW) nos dois tabuleiros via MapLayout
     * 4. Monta as frotas (playerShips e computerShips) conforme as regras do mapa:
     *  | MapType | Dimensão | Frota                        |
     *  |---------|----------|------------------------------|
     *  | Açude   | 5 × 5    | 1P + 1M + 1M  (3 navios)     |
     *  | LAGO    | 8 × 8    | 2P + 2M + 1G  (5 navios)     |
     *  | OCEANO  | 10 × 10  | 3P + 2M + 2G  (7 navios)     |
     * 5. Define o turno inicial como Turno::PLAYER.
     *
     * O timer NÃO é iniciado aqui, chame startTimer() quando a partida
     * efetivamente começar (após o posicionamento dos navios)
     *
     * @param map Tipo do mapa selecionado pelo jogador.
     *
     * @see MapType
     * @see MapLayout::apply()
     * @see startTimer()
     *
     * @author Georis
     * @date 24/07/2026
     */
    GameState(MapType map);


    // --- Turno ----------------------------------->

    /**
     * @brief Retorna de quem é a vez atual no jogo
     * @return Turno::PLAYER se for a vez do jogador humano;
     *         Turno::COMPUTER se for a vez do computador.
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




    // -- Navios ----------------------------------->

    /**
     * @brief Retorna referência à frota do jogador
     * @details Usado pelo GameController para registrar acertos em navios
     * (ship.hit()) e verificar afundamentos (ship.isSunk()).
     * Usado pelo AutoPlacer para posicionar os navios no board.
     * @return std::vector<Ship>& Lista modificável de navios do jogador.
     * @author Georis
     * @date 24/07/2026
     */
    std::vector<Ship>& getPlayerShips();


    /**
     * @brief Retorna referência à frota do computador
     * @details Usado pela IA para posicionar seus navios e pelo
     * GameController para detectar afundamentos após tiro do jogador.
     * @return std::vector<Ship>& Lista modificável de navios do computador.
     * @author Georis
     * @date 24/07/2026
     */
    std::vector<Ship>& getComputerShips();


    /**
     * @brief Retorna o tipo de mapa desta partida.
     * @return O MapType passado no construtor.
     * @author Georis
     * @date 24/07/2026
     */
    MapType getMapType() const;



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
    MapType mapType;            ///< Mapa desta partida — determinou tamanho e frota.
    Board playerBoard;          ///< Tabuleiro do jogador humano.
    Board computerBoard;        ///< Tabuleiro do computador.
    std::vector<Ship> playerShips;   ///< Frota do jogador.
    std::vector<Ship> computerShips; ///< Frota do computador.
    Turno currentTurn;          ///< De quem é a vez atual (player ou computer)

    /**
     * @brief Momento em que o timer da partida foi iniciado
     * @details Utilizado para calcular o tempo total de jogo.
     * Inicializado por startTimer().
     */
    std::chrono::steady_clock::time_point startTime;

    /**
     * @brief Retorna as dimensões do tabuleiro para um dado MapType.
     * @param map Tipo do mapa.
     * @return Par {rows, cols}.
     * @author Georis
     * @date 24/07/2026
     */
    static std::pair<int,int> boardSize(MapType map);

    /**
     * @brief Monta a frota correta para um dado MapType.
     * @details Todos os navios são criados em posição (0,0) horizontal —
     * o posicionamento real ocorre depois via PlacementScreen ou AutoPlacer.
     * @param map Tipo do mapa.
     * @return Vetor de Ship pronto para ser posicionado no board.
     * @author Georis
     * @date 24/07/2026
     */
    static std::vector<Ship> buildFleet(MapType map);

    };