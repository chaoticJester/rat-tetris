#include "Game.h"
#include "SRS.h"

Game::Game() : current(pieceGenerator.next()), currentHold(Tetromino(PieceType::Empty)) {}

bool Game::isValidPosition(const Tetromino& piece) const {
    std::array<Point, 4> currentPosi = piece.getBlocks();
    for(int i = 0; i < 4; i++) {
        if(!board.isEmpty(currentPosi[i].x, currentPosi[i].y)) {
            return false;
        }
    }
    return true;
}

std::array<Point,4> Game::getCurrentBlocks() const {
    return current.getBlocks();
}

bool Game::spawn() {
    current = Tetromino(pieceGenerator.next());
    if (!isValidPosition(current)) { // ชิ้นใหม่ทับบล็อกเดิมตั้งแต่เกิด
        gameOver = true;    
        return false   ;      // = จบเกม
    } 
    return true;
}

bool Game::moveRight() {
    if(gameOver) return false;
    Tetromino copy = current;       
    copy.move(1, 0);               
    if (isValidPosition(copy)) {  
        current = copy;
        return true;
    }
    return false;
}

bool Game::moveLeft() {
    if(gameOver) return false;
    Tetromino copy = current;
    copy.move(-1, 0);
    if (isValidPosition(copy)) {  
        current = copy;
        return true;
    }
    return false;
}

bool Game::softDrop() {
    if(gameOver) return false;
    Tetromino copy = current;
    copy.move(0, 1);
    if (isValidPosition(copy)) {  
        current = copy;
        return true;
    }
    return false;
}

bool Game::rotate(Direction dir) {
    if(gameOver) return false;
    if(current.getPieceType() == PieceType::Empty) return false;
    if(current.getPieceType() == PieceType::O) return true;

    int from = (int)current.getRotationState();
    Tetromino copy = current;
    copy.rotate(dir);
    int to = (int)copy.getRotationState();


    const std::array<std::array<std::array<Point,5>,4>,4>& table = (current.getPieceType() == PieceType::I) ? KICK_I : KICK_JLSTZ; 

    for(int i = 0; i <= 4; i++) {
        Tetromino test = copy;
        test.move(table[from][to][i].x, table[from][to][i].y);
        if(isValidPosition(test)) {
            current = test;
            return true;
        }
    }
    return false;
}

void Game::lock() {
    board.lockPiece(current.getBlocks(), current.getPieceType());
    board.clearLines();
    canHold = true; 
    spawn();
}

bool Game::hardDrop() {
    if(gameOver) return false;
    bool flag = false;
    while(true) {
        if(!softDrop()) break;
        flag = true;
    }
    lock();
    return flag;
}

void Game::tick() {
    if(gameOver) return;
    if (!softDrop())   // ขยับลงไม่ได้ = ถึงพื้น/ชนแล้ว
        lock();
}

bool Game::isGameOver() const {
    return this->gameOver;
}

bool Game::hold() {
    if(gameOver) return false;
    if(!canHold) return false;
    canHold = false;                 

    if(currentHold.getPieceType() == PieceType::Empty) {
        currentHold = Tetromino(current.getPieceType());
        return spawn();              
    } else {
        PieceType currentType = current.getPieceType();
        PieceType holdType = currentHold.getPieceType();
        current = Tetromino(holdType);
        currentHold = Tetromino(currentType);
        if(!isValidPosition(current)) gameOver = true;
    }
    return true;
}