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
    /**
     * @brief Resultado final da partida, retornado por run() quando um dos
     * lados vence ou a janela é fechada.
     * @details Consumido por main.cpp para montar o GameData e calcular a
     * pontuação final via ScoreCalculator::calculate().
     */
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
    /**
     * @brief Resultado de um único tiro básico, resolvido por resolveShot().
     * @details Usado apenas para os tiros do computador (sempre básicos, uma
     * célula por vez) — os tiros do jogador, que podem usar armas especiais
     * com múltiplas células, retornam GameController::FireResult em vez disso.
     */
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

    sf::Clock aiDelayClock;

    std::string statusMessage;

    int hits;          
    int misses;
    int shipsDestroyed;

    /**
     * @brief Processa os eventos da janela referentes ao turno do jogador.
     * @details Ignorado por completo enquanto não for o turno do jogador
     * (Turno::PLAYER), exceto pelo fechamento da janela. Um clique esquerdo
     * é tratado, nesta ordem: (1) clique num botão da barra de armas, que só
     * troca a arma selecionada sem consumir o turno; (2) clique numa célula
     * válida do tabuleiro inimigo, que dispara a arma selecionada via
     * GameController::fire(), atualiza as estatísticas (hits/misses), monta
     * a mensagem de status e, se acertar, verifica se algum navio afundou
     * para conceder uma arma especial via GameController::awardRandomWeapon().
     * Ao final de um disparo válido, passa o turno para o computador.
     */
    void processEvents();

    /**
     * @brief Executa o tiro do computador quando chega o seu turno.
     * @details Não faz nada enquanto o turno for do jogador, nem antes do
     * pequeno delay (aiDelayClock) que dá tempo do jogador acompanhar a
     * jogada anterior na tela. Quando dispara, sempre usa um tiro básico
     * (a IA não tem acesso às armas especiais), resolvido via resolveShot(),
     * e alimenta a AIPlayer com o resultado (AIPlayer::onShotResult() e,
     * se afundar um navio, AIPlayer::onShipSunk()) para ela decidir o
     * próximo alvo. Ao final, passa o turno de volta para o jogador.
     */
    void update();

    /**
     * @brief Desenha todos os elementos da tela de batalha.
     * @details Limpa a janela, desenha o cenário decorativo (mar/grade),
     * o título, a mensagem de status atual, a barra de armas
     * (drawWeaponBar()), os dois tabuleiros (com os rótulos de cada frota),
     * o painel de status de cada frota (drawFleetPanel()) e, por fim, a
     * moldura do HUD.
     */
    void render();

    /**
     * @brief Aplica um tiro básico numa única célula do board informado.
     * @details Chama Board::shoot() e, se o resultado for HIT, localiza o
     * Ship correspondente na lista para propagar o dano (Ship::hit()) e
     * verificar se ele afundou (Ship::isSunk()). Usado apenas para os tiros
     * do computador — os tiros do jogador (que podem usar armas especiais)
     * passam por GameController::fire() em vez deste método.
     * @param board Tabuleiro onde o tiro será aplicado.
     * @param ships Frota correspondente ao board, para propagar o dano.
     * @param row Linha da célula atingida.
     * @param col Coluna da célula atingida.
     * @return ShotOutcome com o estado da célula após o tiro e se algum navio afundou.
     */
    ShotOutcome resolveShot(Board& board, std::vector<Ship>& ships, int row, int col);

    /**
     * @brief Converte uma coordenada (linha, coluna) para o rótulo exibido no tabuleiro.
     * @details Usado para montar as mensagens de status (ex.: linha 3,
     * coluna 1 vira "B4": a coluna 1 vira a letra 'B' e a linha 3 vira "4",
     * já que ambas são exibidas a partir de 1 para o jogador).
     * @param row Linha da célula (índice a partir de 0).
     * @param col Coluna da célula (índice a partir de 0).
     * @return String no formato "<letra da coluna><número da linha>".
     */
    static std::string cellLabel(int row, int col);

    /**
     * @brief Desenha o painel de status de uma frota, abaixo do seu tabuleiro.
     * @details Composto por um cabeçalho colorido com o título da frota e um
     * corpo onde cada navio aparece "em pé", ordenado do menor para o maior,
     * como uma pilha de quadrados representando seus segmentos: os
     * segmentos já atingidos somem primeiro (a "vida" esvazia de cima para
     * baixo) e navios completamente afundados ficam só com o contorno, sem
     * preenchimento.
     * @param x Posição X (em pixels) do canto esquerdo do painel.
     * @param panelWidth Largura do painel, em pixels (igual à largura do tabuleiro correspondente).
     * @param top Posição Y (em pixels) do topo do painel.
     * @param title Título exibido no cabeçalho do painel (ex.: "NOSSOS NAVIOS").
     * @param headerColor Cor de fundo do cabeçalho.
     * @param aliveColor Cor dos segmentos de navio ainda intactos.
     * @param ships Frota a ser exibida no painel.
     */
    void drawFleetPanel(float x, float panelWidth, float top, const std::string& title,
                        sf::Color headerColor, sf::Color aliveColor, const std::vector<Ship>& ships);

    /**
     * @brief Desenha os três botões de seleção de arma (Básico/Míssil/Torpedo).
     * @details Míssil e Torpedo aparecem desativados (esmaecidos e não
     * clicáveis) enquanto o jogador não tiver estoque de nenhum dos dois,
     * que só é ganho ao afundar um navio inimigo (ver
     * GameController::awardRandomWeapon()). A arma atualmente selecionada
     * (GameController::getSelectedWeapon()) é destacada com uma borda
     * dourada; o botão sob o cursor, quando habilitado, também é realçado.
     */
    void drawWeaponBar();

    /**
     * @brief Nome em português (sem acento) de uma arma especial.
     * @details Usado para montar as mensagens de status quando o jogador
     * ganha uma arma (ex.: "Voce ganhou 1 missil!").
     * @param weapon Tipo de arma (espera-se MISSILE ou TORPEDO).
     * @return "missil", "torpedo", ou string vazia para WeaponType::BASIC.
     */
    static std::string weaponName(WeaponType weapon);
};
