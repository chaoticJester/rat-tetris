//Tetromino.h
#pragma once

#include "Types.h"
#include <array>

// ค่าสถานะการหมุนของชิ้นส่วน
enum class RotationState {
    SPAWN, R, FLIP, L
};

//ทิศการหมุน
enum class Direction {
    CW, CCW, FLIP
};


class Tetromino {
    private:
        PieceType pieceType;
        RotationState rotateState;
        Point position;
        
        // ShapeDataTable[pieceType][state] = พิกัด local 4 จุดของบล็อก
        // (0,0) = มุมซ้ายบนของกล่อง (I ใช้ 4x4, JLSTZ ใช้ 3x3)
        // นิยามค่าจริงใน Tetromino.cpp
        static const std::array<std::array<std::array<Point, 4>, 4>, 8> ShapeDataTable;
    public:
        Tetromino(PieceType pieceType);

        //คืนพิกัดจริงบน board
        std::array<Point, 4> getBlocks() const;
        void move(int dx, int dy);
        void rotate(Direction dir);
        PieceType getPieceType() const;
        RotationState getRotationState() const;
};