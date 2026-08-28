#include "../Board.h"
#include <iostream>
#include <cassert>

int main() {
    Board b;

    // 1. constructor: ทุกช่องควรว่างตอนเริ่ม
    assert(b.isEmpty(0, 0));
    assert(b.isEmpty(9, 39));
    std::cout << "[OK] constructor cleard empty grids.\n";

    // 2. isInGrid: ขอบเขต 4 ด้าน
    assert(b.isInGrid(0, 0));       // มุมซ้ายบน
    assert(b.isInGrid(9, 39));      // มุมขวาล่าง
    assert(!b.isInGrid(-1, 0));     // ซ้ายเกิน
    assert(!b.isInGrid(10, 0));     // ขวาเกิน (WIDTH=10 → index สูงสุด 9)
    assert(!b.isInGrid(0, -1));     // บนเกิน
    assert(!b.isInGrid(0, 40));     // ล่างเกิน (TOTAL_HEIGHT=40 → index สูงสุด 39)
    std::cout << "[OK] isInGrid checked all 4 corners\n";

    // 3. setCell / getCell
    b.setCell(3, 5, PieceType::T);
    assert(b.getCell(3, 5).type == PieceType::T);
    assert(!b.isEmpty(3, 5));       // ช่องที่เซ็ตแล้วไม่ว่าง
    std::cout << "[OK] setCell/getCell worked correctly\n";

    // 4. isEmpty คืน false เมื่อออกนอกกริด (กัน crash)
    assert(!b.isEmpty(-1, 0));
    assert(!b.isEmpty(100, 100));
    std::cout << "[OK] isEmpty outside of grid = false (didn't crash)\n";

    // 5. isRowFull
    assert(!b.isRowFull(0));        // แถวว่าง → ไม่เต็ม
    for (int x = 0; x < 10; x++) b.setCell(x, 0, PieceType::I);
    assert(b.isRowFull(0));         // เติมเต็มแถว → เต็ม
    std::cout << "[OK] isRowFull check fulled row correctly\n";

    std::cout << "\npassed all tests!\n";
    return 0;
}