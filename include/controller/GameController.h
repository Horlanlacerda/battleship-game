#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include "model/Board.h"
#include "model/Ship.h"


/**
 * @brief Tipos de armas disponíveis para o jogador (RF02).
 * @details Agrupa as armas do jogo: o disparo básico padrão, o míssil que atinge
 * uma área de 2x2 células e o torpedo que limpa uma linha inteira.
 * @author Gabi
 * @version 1.0
 * @date 12/08/2026
 */
enum class WeaponType {
    BASIC,   ///< Tiro único em uma única célula.
    MISSILE, ///< Míssil de área (2x2 células).
    TORPEDO  ///< Torpedo de linha (atinge todas as colunas da linha).
};

/**
 * @brief Converte cliques do mouse em coordenadas de tabuleiro (linha, coluna).
 * @details O GameController não processa regras de jogo nem desenha nada —
 * ele só traduz posições de tela (pixels) para posições de matriz, usando
 * os mesmos parâmetros de tamanho/deslocamento que o BoardRenderer usa
 * para desenhar, garantindo que clique e desenho fiquem sincronizados.
 * @author Gabi
 * @version 1.1
 * @date 17/07/2026
 */
class GameController {
public:
    /**
     * @brief Constrói o controller com os mesmos parâmetros visuais do tabuleiro.
     * @param cellSize Tamanho em pixels de cada célula (deve bater com o BoardRenderer).
     * @param offsetX Deslocamento horizontal onde o tabuleiro começa na tela.
     * @param offsetY Deslocamento vertical onde o tabuleiro começa na tela.
     * @param rows Número de linhas do tabuleiro (para validar limites).
     * @param cols Número de colunas do tabuleiro (para validar limites).
     */
    GameController(float cellSize, float offsetX, float offsetY, int rows, int cols);

    /**
     * @brief Converte uma posição de clique (em pixels) para coordenada de matriz.
     * @param mousePos Posição do clique em pixels, geralmente vinda de sf::Mouse::getPosition(window).
     * @param outRow Referência onde a linha calculada será armazenada, se o clique for válido.
     * @param outCol Referência onde a coluna calculada será armazenada, se o clique for válido.
     * @return true se o clique caiu dentro dos limites do tabuleiro; false caso contrário.
     */
    bool screenToBoardCoord(sf::Vector2i mousePos, int& outRow, int& outCol) const;

// --- Gerenciamento de Armas ----------------------------------->

    /**
     * @brief Define a arma selecionada para o próximo disparo.
     * @details Só altera a seleção para armas especiais se houver munição disponível no estoque.
     * @param weapon Tipo de arma a ser selecionada (BASIC, MISSILE ou TORPEDO).
     */
    void selectWeapon(WeaponType weapon);

    /**
     * @brief Retorna a arma atualmente selecionada pelo jogador.
     * @return WeaponType selecionado no momento.
     */
    WeaponType getSelectedWeapon() const;

    /**
     * @brief Retorna a quantidade de mísseis restantes no estoque.
     * @return Número de mísseis disponíveis.
     */
    int getMissilesLeft() const;

    /**
     * @brief Retorna a quantidade de torpedos restantes no estoque.
     * @return Número de torpedos disponíveis.
     */
    int getTorpedoesLeft() const;

    /**
     * @brief Concede ao jogador uma arma especial escolhida aleatoriamente
     * entre míssil e torpedo, incrementando o estoque correspondente.
     * @details Chamado pela tela de jogo sempre que o jogador afunda um
     * navio inimigo — é assim que o jogador "ganha" acesso às armas
     * especiais, que começam zeradas.
     * @return O WeaponType sorteado (MISSILE ou TORPEDO), para a tela de
     * jogo poder montar a mensagem "Voce ganhou 1 missil/torpedo!".
     */
    WeaponType awardRandomWeapon();

    // --- Processamento de Disparos -------------------------------->

    /**
     * @brief Executa um disparo básico em uma única célula do tabuleiro inimigo.
     * @param enemy Referência ao tabuleiro do adversário.
     * @param row Linha do alvo.
     * @param col Coluna do alvo.
     * @return CellState com o resultado do disparo (HIT, WATER, etc.).
     */
    CellState fireBasic(Board& enemy, int row, int col);

    /**
     * @brief Dispara um míssil atingindo um quadrante 2x2 a partir da célula clicada.
     * @details Consome 1 míssil do estoque e reseta a arma selecionada para BASIC.
     * @param enemy Referência ao tabuleiro do adversário.
     * @param row Linha inicial (canto superior esquerdo da área 2x2).
     * @param col Coluna inicial (canto superior esquerdo da área 2x2).
     * @return Quantidade de acertos em navios (HIT) gerados pelo ataque.
     */
    int fireMissile(Board& enemy, int row, int col);

    /**
     * @brief Dispara um torpedo atingindo todas as colunas da linha especificada.
     * @details Consome 1 torpedo do estoque e reseta a arma selecionada para BASIC.
     * @param enemy Referência ao tabuleiro do adversário.
     * @param row Linha a ser atacada por completo.
     * @param col Coluna clicada (mantida para alinhamento de interface).
     * @return Quantidade de acertos em navios (HIT) gerados ao longo da linha.
     */
    int fireTorpedo(Board& enemy, int row, int col);

    /**
     * @brief Resultado consolidado de um disparo processado por fire(),
     * já cobrindo todas as células afetadas pela arma usada (1 para
     * BASIC, até 4 para MISSILE, a linha inteira para TORPEDO).
     */
    struct FireResult {
        int hits = 0;        ///< Quantas células atingidas resultaram em acerto (HIT).
        int misses = 0;       ///< Quantas células atingidas resultaram em água (MISS).
        int shipsSunk = 0;    ///< Quantos navios foram completamente afundados por este disparo.
    };

    /**
     * @brief Processa um disparo da arma atualmente selecionada contra o
     * tabuleiro e a frota inimiga, cobrindo de uma vez todas as células
     * afetadas (diferente de fireBasic/fireMissile/fireTorpedo, que só
     * atualizam o Board — aqui o dano também é propagado para os Ship da
     * frota, para a tela de jogo saber quando um navio afunda).
     * @details Consome a munição da arma usada (se especial) e volta a
     * seleção para BASIC, assim como fireMissile()/fireTorpedo().
     * @param enemy Tabuleiro do adversário.
     * @param enemyShips Frota do adversário, para propagar dano e detectar afundamentos.
     * @param row Linha clicada (âncora do disparo).
     * @param col Coluna clicada (âncora do disparo).
     * @return FireResult com acertos, erros e navios afundados neste disparo.
     */
    FireResult fire(Board& enemy, std::vector<Ship>& enemyShips, int row, int col);

private:
    float cellSize; ///< Tamanho em pixels de cada célula.
    float offsetX;  ///< Deslocamento horizontal do tabuleiro na janela.
    float offsetY;  ///< Deslocamento vertical do tabuleiro na janela.
    int rows;       ///< Total de linhas do tabuleiro.
    int cols;       ///< Total de colunas do tabuleiro.

    WeaponType selectedWeapon = WeaponType::BASIC; ///< Arma selecionada atualmente.
    int missilesLeft = 0;                          ///< Mísseis restantes (começa zerado, ganho ao afundar navios).
    int torpedoesLeft = 0;                         ///< Torpedos restantes (começa zerado, ganho ao afundar navios).
};
