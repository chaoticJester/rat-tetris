// test_tetromino.cpp
// เทสยืนยันว่า ShapeDataTable ของทุกชิ้นทุก state ถูกต้อง
//
// วิธี compile & run:
//   g++ -std=c++17 test_tetromino.cpp Tetromino.cpp -o test_tetromino
//   ./test_tetromino
//
// หลักการ: สร้างชิ้นแต่ละชนิด แล้ว print รูปลงกริด ASCII 4x4 ครบทั้ง 4 state
// (หมุน CW ไปเรื่อยๆ) เอาไปเทียบด้วยตากับที่ควรจะเป็นตาม SRS
//
// หมายเหตุ: เทสย้ายชิ้นกลับมาที่ (0,0) ด้วย move(-3,-21) ก่อน
// เพื่อให้ getBlocks() คืนพิกัด local (0..3) จะได้ print ในกริดเล็กๆ ได้พอดี

#include "../Tetromino.h"
#include <iostream>
#include <string>
#include <array>

// แปลงชื่อ state เป็นข้อความ (ไว้ทำ label)
static const char* stateName(RotationState s) {
    switch (s) {
        case RotationState::SPAWN: return "SPAWN";
        case RotationState::R:     return "R";
        case RotationState::FLIP:  return "FLIP";
        case RotationState::L:     return "L";
    }
    return "?";
}

// แปลงชนิดชิ้นเป็นตัวอักษร
static const char* pieceName(PieceType p) {
    switch (p) {
        case PieceType::I: return "I";
        case PieceType::O: return "O";
        case PieceType::T: return "T";
        case PieceType::S: return "S";
        case PieceType::Z: return "Z";
        case PieceType::J: return "J";
        case PieceType::L: return "L";
        default:           return "?";
    }
}

// สร้างกริด ASCII 4x4 ของชิ้นใน state ปัจจุบัน
// (คาดว่า position = (0,0) แล้ว → getBlocks() จึงเป็นพิกัด local)
static std::array<std::string, 4> renderGrid(const Tetromino& t) {
    std::array<std::string, 4> grid;
    for (auto& row : grid) row = "....";          // เริ่มด้วยจุดว่างทั้งหมด
    for (const Point& p : t.getBlocks()) {
        if (p.x >= 0 && p.x < 4 && p.y >= 0 && p.y < 4) {
            grid[p.y][p.x] = '#';                 // grid[แถว][คอลัมน์] = [y][x]
        }
    }
    return grid;
}

// print ชิ้นเดียว ครบ 4 state วางเรียงข้างกันเพื่อเทียบง่าย
static void testPiece(PieceType type) {
    Tetromino t(type);
    t.move(-3, -21);   // ล้าง spawn position → เหลือ (0,0)

    std::array<std::array<std::string, 4>, 4> grids;  // กริดของทั้ง 4 state
    std::array<std::string, 4> labels;

    for (int s = 0; s < 4; s++) {
        labels[s] = stateName(t.getRotationState());
        grids[s]  = renderGrid(t);
        t.rotate(Direction::CW);                       // หมุนไป state ถัดไป
    }

    std::cout << "=== Piece " << pieceName(type) << " ===\n";

    // แถวป้ายชื่อ state (จัดให้ตรงคอลัมน์ กว้างช่องละ 6)
    for (int s = 0; s < 4; s++) {
        std::string lbl = labels[s];
        lbl.resize(6, ' ');
        std::cout << lbl;
    }
    std::cout << "\n";

    // 4 แถวของกริด — แต่ละแถวโชว์ทั้ง 4 state เรียงกัน
    for (int row = 0; row < 4; row++) {
        for (int s = 0; s < 4; s++) {
            std::cout << grids[s][row] << "  ";        // กริด 4 ตัว + เว้น 2 = 6
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    PieceType pieces[] = {
        PieceType::I, PieceType::O, PieceType::T,
        PieceType::S, PieceType::Z, PieceType::J, PieceType::L
    };
    for (PieceType p : pieces) {
        testPiece(p);
    }
    return 0;
}