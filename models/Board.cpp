#include "Board.h"

Board::Board() {}

bool Board::isInGrid(int x, int y) const {
    return x >= 0 && x < WIDTH && y >= 0 && y < TOTAL_HEIGHT;
}

bool Board::isEmpty(int x, int y) const {
    return isInGrid(x, y) && grid2D[y][x].isEmpty();
}

const Cell& Board::getCell(int x, int y) const {
    return grid2D[y][x];
}

void Board::setCell(int x, int y, PieceType type) {
    grid2D[y][x].type = type;
}

int Board::getWidth() const {
    return WIDTH;
}

int Board::getVisibleHeight() const {
    return VISIBLE_HEIGHT;
}

int Board::getTotalHeight() const {
    return TOTAL_HEIGHT;
}

bool Board::isRowFull(int y) const {
    for(int x = 0; x < WIDTH; x++) {
        if(grid2D[y][x].isEmpty()) return false;
    }
    return true;
}

void Board::lockPiece(const std::array<Point,4>& blocks, PieceType type) {
    for(int i = 0; i < 4; i++) {
        if(!isInGrid(blocks[i].x, blocks[i].y)) return;
        setCell(blocks[i].x, blocks[i].y, type);
    }
}

int Board::clearLines() {
    int count = 0;
    for (int y = TOTAL_HEIGHT - 1; y >= 0; ) {
        if (isRowFull(y)) {
            for (int i = y; i > 0; i--) grid2D[i] = grid2D[i-1];   
            grid2D[0] = {};                                         
            count++;
        } else {
            y--;    
        }
    }
    return count;
}