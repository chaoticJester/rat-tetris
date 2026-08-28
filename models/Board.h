// Board.h
#pragma once

#include "Types.h"
#include "Cell.h"
#include <array>


class Board {
    private:
        static constexpr int WIDTH = 10;
        static constexpr int VISIBLE_HEIGHT = 20;
        static constexpr int TOTAL_HEIGHT = 40;
        std::array<std::array<Cell, WIDTH>, TOTAL_HEIGHT> grid2D;
    public:
        Board();
        bool isInGrid(int x, int y) const;
        bool isEmpty(int x, int y) const;
        const Cell& getCell(int x, int y) const;
        void setCell(int x, int y, PieceType type);
        int getWidth() const;
        int getVisibleHeight() const;
        int getTotalHeight() const;
        bool isRowFull(int y) const;
        void lockPiece();
        void clearLines();
};