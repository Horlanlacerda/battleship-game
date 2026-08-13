#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Database.h"

/**
 * @brief Telas de ranking: lista de melhores pontuações e tela de fim de partida.
 * @details Segue o mesmo padrão visual das demais telas (MenuScreen,
 * InstructionsScreen...), reutilizando o cenário e os elementos
 * compartilhados em UiTheme. Usa a Database para ler e persistir as
 * pontuações do ranking.
 *
 * @author Suelle
 * @date 12/08/2026
 */
class RankingScreen {
public:
    /**
     * @brief Constrói a tela de ranking.
     * @details Carrega a fonte usada nos textos da tela.
     * @param window Janela principal da aplicação.
     * @param db Banco de dados de onde o ranking é lido e onde novas pontuações são salvas.
     */
    RankingScreen(sf::RenderWindow& window, Database& db);

    /**
     * @brief Mostra a lista das melhores pontuações de um mapa e bloqueia até o jogador voltar.
     * @details Busca as 10 melhores pontuações do mapa via Database::getTopScores()
     * e desenha um painel com a posição, nome, pontuação e tempo de cada
     * uma (o 1º colocado em destaque dourado), ou uma mensagem indicando
     * que ainda não há resultados. Roda em loop até a janela ser fechada
     * ou ESC ser pressionado.
     * @param mapType Tipo de mapa cujo ranking será exibido ("ACUDE", "LAGO" ou "OCEANO").
     */
    void showRanking(const std::string& mapType);

    /**
     * @brief Mostra a tela de fim de partida e bloqueia até o jogador digitar seu nome.
     * @details Exibe o resultado (vitória ou derrota), o mapa jogado, a
     * pontuação e o tempo decorrido, e um campo de texto onde o jogador
     * digita seu nome (até 16 caracteres) para salvar no ranking. Roda em
     * loop até ENTER ser pressionado ou a janela ser fechada.
     * @param score Pontuação final da partida (já calculada por ScoreCalculator).
     * @param elapsedSec Tempo total da partida, em segundos.
     * @param playerWon true se o jogador venceu a partida, false se perdeu.
     * @param mapType Tipo de mapa jogado, exibido na tela.
     * @return O nome digitado pelo jogador, ou "Anonimo" se nada for digitado
     * ou a janela for fechada antes de confirmar.
     */
    std::string showGameOver(int score, int elapsedSec, bool playerWon, const std::string& mapType);

private:
    sf::RenderWindow& window; 
    Database& db; 
    sf::Font font;
};
