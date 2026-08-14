#pragma once

/**
 * @brief Estatísticas de uma partida encerrada, usadas para calcular a pontuação final.
 * @details Preenchido a partir do GameScreen::Outcome retornado por GameScreen::run()
 * ao fim da partida (vitória ou derrota), e passado para ScoreCalculator::calculate().
 *
 * @author Suelle
 * @date 09/07/2026
 */
struct GameData {
    int hits;              
    int misses;               
    int shipsDestroyed;      
    int alliedShipsSurvived;  
    int elapsedSeconds;   
};

/**
 * @brief Calcula a pontuação final do jogador a partir das estatísticas da partida.
 * @details Classe utilitária (só métodos estáticos, sem estado) usada por main.cpp
 * logo após o fim de uma partida, para transformar o GameData em um score que é
 * exibido na tela de fim de jogo (RankingScreen::showGameOver()) e salvo no
 * ranking (Database::saveResult()).
 *
 * @author Suelle
 * @date 09/07/2026
 */
class ScoreCalculator {
public:
    /**
     * @brief Calcula a pontuação da partida com base nos acertos, erros e desempenho do jogador.
     * @details A pontuação é composta por ganhos e perdas:
     * - Ganhos: +100 por acerto (hits), +200 por navio inimigo afundado (shipsDestroyed)
     *   e +150 por navio aliado sobrevivente (alliedShipsSurvived).
     * - Perdas: -20 por tiro errado (misses) e -0.5 por segundo de duração da
     *   partida (elapsedSeconds), incentivando partidas mais rápidas.
     *
     * O resultado nunca é negativo: caso a soma dos descontos supere os ganhos,
     * a pontuação final é limitada a 0.
     *
     * @param data Estatísticas coletadas ao longo da partida (ver GameData).
     * @return Pontuação final do jogador, sempre maior ou igual a zero.
     */
    static int calculate(const GameData& data);
};