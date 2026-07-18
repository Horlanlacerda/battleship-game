#include "../../include/model/ScoreCalculator.h"
#include <algorithm> //para o max

int ScoreCalculator::calculate(const GameData& data) {
    int score = 0;

    //ganhos
    score += data.hits * 100;
    score += data.shipsDestroyed * 200;
    score += data.alliedShipsSurvived * 150;

    //percas
    score -= data.misses * 20;
    score -= static_cast<int>(data.elapsedSeconds * 0.5);
    return std::max(0, score);
}