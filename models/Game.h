//Game.h
#pragma once 

#include "Board.h"
#include "../handlers/PieceGenerator.h"
#include "Tetromino.h"
#include <array>

class Game {

    private:
        Board board;
        PieceGenerator pieceGenerator;
        Tetromino current;
        bool gameOver = false;
    
    public:
        Game();
        bool isValidPosition(const Tetromino& piece) const;
        std::array<Point,4> getCurrentBlocks() const;
        void spawn();
        bool moveRight();
        bool moveLeft();
        bool softDrop();
        bool rotate(Direction dir);
        void lock();
        bool hardDrop();
        void tick();
        bool isGameOver() const;
};
