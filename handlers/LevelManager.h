//ScoreManager.h
#pragma once

#include <array>

static const std::array<int, 5> SCORE_TABLE = {0, 100, 300, 500, 800};
static const double DELAY_TABLE[] = {
        1.0, 0.84, 0.68, 0.51, 0.35,     // level 1-5
        0.29, 0.23, 0.18, 0.12, 0.06,    // level 6-10
        0.052, 0.044, 0.036, 0.028, 0.02,
    };

class LevelManager {
    private:
        int score;
        int totalLines;
        int level;
        double fallDelay;
        double lockDelay;
    public:
        LevelManager();
        void addLines(int n);
        void levelUp();
        int getScore() const;
        int getLevel() const;
        int getTotalLines() const;
        double getFallDelay() const;
        double getLockDelay() const;
        double computeDelay(int lv) const;
};