#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Board.h"
#include "Ship.h"
#include "BoardRenderer.h"
#include "GameController.h"
#include "AIPlayer.h"
#include "GameState.h"

/**
 * @brief Tela de batalha: jogador humano contra a IA
 *
 * @details Mostra os dois tabuleiros lado a lado (o do jogador, com os
 * navios visíveis, e o do computador, com os navios escondidos) e alterna
 * os turnos entre PLAYER e COMPUTER usando o Turno já controlado pelo
 * GameState.
 *
 * A cada tiro (seja do humano ou da IA), a tela resolve o disparo no
 * Board correspondente e propaga o resultado para o Ship atingido.
 * 
 * A troca de modo da IA (Hunting -> Targeting -> Hunting) já é decidida
 * internamente pela própria AIPlayer; esta tela só precisa alimentá-la
 * corretamente logo após cada disparo do computador:
 * - AIPlayer::onShotResult() é chamado com o CellState real do tiro
 *   (aí a IA entra em modo Targeting, se o resultado for HIT).
 * - AIPlayer::onShipSunk() é chamado quando o navio atingido afunda
 *   (é aí que a IA volta para o modo Hunting).
 *
 * @author Suelle
 * @date 12/08/2026
 */
class GameScreen {
public:
    struct Outcome {
        GameResult result;            
        int hits;                      
        int misses;                    
        int shipsDestroyed;           
        int alliedShipsSurvived;      
        int elapsedSeconds;           
    };

    /**
     * @brief Constrói a tela de batalha.
     * @details Posiciona automaticamente a frota do computador (via
     * AutoPlacer), a frota do jogador já deve ter sido posicionada antes
     * (pelo PlacementScreen) usando o mesmo GameState.
     * @param window Janela principal da aplicação.
     * @param gameState Estado da partida (tabuleiros, frotas e turno).
     */
    GameScreen(sf::RenderWindow& window, GameState& gameState);

    /**
     * @brief Executa o loop da partida até um dos lados vencer ou a janela fechar.
     * @return Outcome com o resultado final e as estatísticas da partida.
     */
    Outcome run();

private:
    struct ShotOutcome {
        CellState state;
        bool shipSunk;
    };

    sf::RenderWindow& window;
    GameState& gameState;
    AIPlayer aiPlayer;
    sf::Font font;

    const int grid;
    const float cellSize;
    const float leftOffsetX;
    const float rightOffsetX;
    const float boardOffsetY;

    BoardRenderer playerBoardRenderer;
    BoardRenderer enemyBoardRenderer;
    GameController enemyController;

    /// Marca um delay antes do tiro da ia (só para o jogador acompanhar a jogada na tela).
    sf::Clock aiDelayClock;

    /// Última mensagem de status exibida.
    std::string statusMessage;

    int hits;
    int misses;
    int shipsDestroyed;

    void processEvents();
    void update();
    void render();

    /// Aplica um tiro no board informado e se acertar propaga o dano
    /// para o Ship correspondente da lista.
    ShotOutcome resolveShot(Board& board, std::vector<Ship>& ships, int row, int col);

    /// Converte uma coordenada (linha, coluna) para o rótulo exibido no
    /// tabuleiro (ex: linha 3, coluna 1 -> "B4"), para as mensagens de status.
    static std::string cellLabel(int row, int col);

    /// Desenha o painel de status de uma frota (cabeçalho + navios em pé,
    /// do menor para o maior, mostrando quantos segmentos de cada um ainda
    /// restam). Navios afundados ficam com o contorno vazio (sem preenchimento).
    void drawFleetPanel(float x, float panelWidth, float top, const std::string& title,
                        sf::Color headerColor, sf::Color aliveColor, const std::vector<Ship>& ships);
};
