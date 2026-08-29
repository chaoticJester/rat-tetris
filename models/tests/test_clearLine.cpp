#include "Board.h"
#include <iostream>
#include <cassert>
int main() {
    // เทส 1: เคลียร์ 1 แถว + บล็อกด้านบนตกลงมา
    {
        Board b;
        for (int x = 0; x < 10; x++) b.setCell(x, 39, PieceType::I);
        b.setCell(3, 38, PieceType::T);
        int cleared = b.clearLines();
        assert(cleared == 1);
        assert(!b.isRowFull(39));
        assert(b.getCell(3, 39).type == PieceType::T);  // T ตกจาก 38 → 39
        std::cout << "[OK] เคลียร์ 1 แถว + บล็อกบนตกลงมาถูก\n";
    }
    // เทส 2: เคลียร์หลายแถวพร้อมกัน (tetris = 4 แถว)
    {
        Board b;
        for (int y = 36; y <= 39; y++)
            for (int x = 0; x < 10; x++) b.setCell(x, y, PieceType::I);
        int cleared = b.clearLines();
        assert(cleared == 4);
        std::cout << "[OK] เคลียร์ 4 แถวรวด (tetris) คืน 4\n";
    }
    // เทส 3: แถวเต็มไม่ติดกัน (มีแถวว่างคั่น) — เช็ค re-check ไม่พลาด
    {
        Board b;
        for (int x = 0; x < 10; x++) b.setCell(x, 39, PieceType::I);  // เต็ม
        for (int x = 0; x < 9;  x++) b.setCell(x, 38, PieceType::Z);  // ไม่เต็ม (ขาด 1)
        for (int x = 0; x < 10; x++) b.setCell(x, 37, PieceType::I);  // เต็ม
        int cleared = b.clearLines();
        assert(cleared == 2);
        // แถว Z ที่เหลือควรตกลงมาอยู่ล่างสุด
        assert(b.getCell(0, 39).type == PieceType::Z);
        assert(b.isEmpty(9, 39));  // ตำแหน่งที่ Z ขาดไป
        std::cout << "[OK] แถวเต็มไม่ติดกัน เคลียร์ถูก + แถวที่เหลือตกลงถูกตำแหน่ง\n";
    }
    // เทส 4: ไม่มีแถวเต็ม → คืน 0
    {
        Board b;
        b.setCell(0, 39, PieceType::I);
        assert(b.clearLines() == 0);
        std::cout << "[OK] ไม่มีแถวเต็ม คืน 0\n";
    }
    std::cout << "\nผ่านทุกเทส!\n";
}