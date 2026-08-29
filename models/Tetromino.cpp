#include "Tetromino.h"

const std::array<std::array<std::array<Point,4>,4>, 8> Tetromino::ShapeDataTable = {{
    // [0] Empty — ปล่อยว่าง
    {},

    // [1] I  
    {{
        {{ {0,1},{1,1},{2,1},{3,1} }},   // SPAWN — เส้นนอน แถว 1
        {{ {2,0},{2,1},{2,2},{2,3} }},   // R     — เส้นตั้ง คอลัมน์ 2
        {{ {0,2},{1,2},{2,2},{3,2} }},   // FLIP  — เส้นนอน แถว 2
        {{ {1,0},{1,1},{1,2},{1,3} }},   // L     — เส้นตั้ง คอลัมน์ 1
    }},

    // [2] O 
    {{
        {{ {1,0},{2,0},{1,1},{2,1} }},   // SPAWN — เส้นนอน แถว 1
        {{ {1,0},{2,0},{1,1},{2,1} }},   // R     — เส้นตั้ง คอลัมน์ 2
        {{ {1,0},{2,0},{1,1},{2,1} }},   // FLIP  — เส้นนอน แถว 2
        {{ {1,0},{2,0},{1,1},{2,1} }},   // L     — เส้นตั้ง คอลัมน์ 1
    }},

    // [3] T (3x3)
    {{
        {{ {1,0},{0,1},{1,1},{2,1} }},   // SPAWN — เส้นนอน แถว 1
        {{ {1,0},{1,1},{2,1},{1,2} }},   // R     — เส้นตั้ง คอลัมน์ 2
        {{ {1,2},{0,1},{1,1},{2,1} }},   // FLIP  — เส้นนอน แถว 2
        {{ {1,0},{1,1},{0,1},{1,2} }},   // L     — เส้นตั้ง คอลัมน์ 1
    }},

    // [4] S (3x3)
    {{
        {{ {1,0},{2,0},{0,1},{1,1} }},   // SPAWN — เส้นนอน แถว 1
        {{ {1,0},{1,1},{2,1},{2,2} }},   // R     — เส้นตั้ง คอลัมน์ 2
        {{ {1,1},{2,1},{0,2},{1,2} }},   // FLIP  — เส้นนอน แถว 2
        {{ {0,0},{0,1},{1,1},{1,2} }},   // L     — เส้นตั้ง คอลัมน์ 1
    }},

    // [5] Z (3x3)
    {{
        {{ {0,0},{1,0},{1,1},{2,1} }},   // SPAWN — เส้นนอน แถว 1
        {{ {2,0},{1,1},{2,1},{1,2} }},   // R     — เส้นตั้ง คอลัมน์ 2
        {{ {0,1},{1,1},{1,2},{2,2} }},   // FLIP  — เส้นนอน แถว 2
        {{ {1,0},{0,1},{1,1},{0,2} }},   // L     — เส้นตั้ง คอลัมน์ 1
    }},

    // [6] J (3x3)
    {{
        {{ {0,0},{0,1},{1,1},{2,1} }},   // SPAWN — เส้นนอน แถว 1
        {{ {1,0},{2,0},{1,1},{1,2} }},   // R     — เส้นตั้ง คอลัมน์ 2
        {{ {0,1},{1,1},{2,1},{2,2} }},   // FLIP  — เส้นนอน แถว 2
        {{ {1,0},{1,1},{0,2},{1,2} }},   // L     — เส้นตั้ง คอลัมน์ 1
    }},

    // [7] L (3x3)
    {{
        {{ {0,1},{1,1},{2,1},{2,0} }},   // SPAWN — เส้นนอน แถว 1
        {{ {1,0},{1,1},{1,2},{2,2} }},   // R     — เส้นตั้ง คอลัมน์ 2
        {{ {0,1},{1,1},{2,1},{0,2} }},   // FLIP  — เส้นนอน แถว 2
        {{ {0,0},{1,0},{1,1},{1,2} }},   // L     — เส้นตั้ง คอลัมน์ 1
    }},
}};

Tetromino::Tetromino(PieceType t) : pieceType(t), rotateState(RotationState::SPAWN), position{3,21} {}

std::array<Point,4> Tetromino::getBlocks() const { 
    std::array<Point,4> local = ShapeDataTable[static_cast<int>(pieceType)][static_cast<int>(rotateState)];
    std::array<Point,4> result;
    for (int i = 0; i < 4; i++) {
        result[i] = { local[i].x + position.x, local[i].y + position.y };
    }
    return result;
}

void Tetromino::move(int dx, int dy) {
    (this->position).x += dx;
    (this->position).y += dy;
}

void Tetromino::rotate(Direction dir) {
    int cur = static_cast<int>(rotateState);
    int step;
    if(dir == Direction::CW) {
        step = 1;
    } else if(dir == Direction::CCW) {
        step = 3;
    } else {
        step = 2;
    }
    this->rotateState = static_cast<RotationState>((cur + step) % 4);
}

PieceType Tetromino::getPieceType() const {
    return this->pieceType;
}

RotationState Tetromino::getRotationState() const {
    return this->rotateState;
}

std::array<Point,4> Tetromino::getLocalBlocks() const {
    return ShapeDataTable[(int)pieceType][(int)rotateState];  // local ล้วน ไม่บวก position
}