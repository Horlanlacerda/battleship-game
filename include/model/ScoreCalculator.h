#ifndef SCORECALCULATOR_H
#define SCORECALCULATOR_H

struct GameData {
    int hits;
    int misses;
    int shipsDestroyed;
    int alliedShipsSurvived;
    int elapsedSeconds;
    // o elapsedSeconds existe pra ser usado depois no ranking, pra quem for mexer no Database.h/.cpp
};

class ScoreCalculator {
public:
    static int calculate(const GameData& data);
};

#endif