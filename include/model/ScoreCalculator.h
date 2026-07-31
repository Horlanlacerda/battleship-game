#pragma once

struct GameData {
    int hits;
    int misses;
    int shipsDestroyed;
    int alliedShipsSurvived;
    int elapsedSeconds;
};

class ScoreCalculator {
public:
    static int calculate(const GameData& data);
};