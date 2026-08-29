//Game.h
#pragma once 

#include "Board.h"
#include "../handlers/PieceGenerator.h"
#include "../handlers/LevelManager.h"
#include "Tetromino.h"
#include <array>

class Game {

    private:
        Board board;
        PieceGenerator pieceGenerator;
        LevelManager levelManager;
        Tetromino current;
        Tetromino currentHold;
        bool gameOver = false;
        bool canHold = true;    
    
    public:
        Game();
        bool isValidPosition(const Tetromino& piece) const;
        std::array<Point,4> getCurrentBlocks() const;
        bool spawn();
        bool moveRight();
        bool moveLeft();
        bool softDrop();
        bool rotate(Direction dir);
        void lock();
        bool hardDrop();
        void tick();
        bool isGameOver() const;
        bool hold();
        PieceType getCurrentType() const;
        PieceType getHoldType() const;
        std::array<PieceType, 5> getNext5Pieces() const;
        const Board& getBoard() const;
        int getScore() const;
        int getLevel() const;
        int getTotalLines() const;
        double getFallDelay() const;   
};
