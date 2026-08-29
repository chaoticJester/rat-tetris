#include "LevelManager.h"

LevelManager::LevelManager() {
    this->score = 0;
    this->totalLines = 0;
    this->level = 1;
    this->fallDelay = 1;
    this->lockDelay = 0.5;
}

void LevelManager::addLines(int n) {
    if (n <= 0) return;
    totalLines += n;
    score += SCORE_TABLE[n] * level;
    levelUp();
}

void LevelManager::levelUp() {
    if(totalLines < (10 * level)) {
        return;
    }
    level++;
    fallDelay = computeDelay(level);
}

int LevelManager::getScore() const {
    return this->score;
}

int LevelManager::getLevel() const {
    return this->level;
}

int LevelManager::getTotalLines() const {
    return this->totalLines;
}

double LevelManager::getFallDelay() const {
    return this->fallDelay;
}

double LevelManager::getLockDelay() const {
    return this->lockDelay;
}

double LevelManager::computeDelay(int lv) const {
    int idx = lv - 1;   // level 1 → index 0
    if (idx >= (int)(sizeof(DELAY_TABLE)/sizeof(DELAY_TABLE[0])))
        return 0.0;   // เกินตาราง = ตกทันที
    return DELAY_TABLE[idx];
}