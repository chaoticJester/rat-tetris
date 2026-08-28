#include "../PieceGenerator.h"
#include <iostream>
#include <array>
#include <cassert>

// เช็กว่า 7 ชิ้นที่ดึงมาติดกัน = ครบทุกชนิดพอดี (คุณสมบัติหลักของ 7-bag)
static void checkBagComplete(std::array<PieceType,7> seven, int bagNo) {
    std::array<int,8> count{};                 // index ด้วย (int)PieceType
    for (PieceType p : seven) count[(int)p]++;
    for (int t = 1; t <= 7; t++)               // 1..7 = I..L (ข้าม Empty=0)
        assert(count[t] == 1 && "แต่ละชนิดต้องมีชิ้นเดียวใน 1 ถุง");
    std::cout << "[OK] ถุงที่ " << bagNo << " มีครบ 7 ชนิด ไม่ซ้ำ\n";
}

int main() {
    PieceGenerator gen;

    // 1. คุณสมบัติ 7-bag: ดึง 7 ชิ้นแรก และ 7 ชิ้นถัดไป ต้องครบทุกชนิดทั้งคู่
    for (int bag = 1; bag <= 3; bag++) {
        std::array<PieceType,7> seven;
        for (int i = 0; i < 7; i++) seven[i] = gen.next();
        checkBagComplete(seven, bag);
    }

    // 2. peek3Next ต้องตรงกับ 3 ชิ้นที่ next() จะให้จริง
    PieceGenerator gen2;
    auto peeked = gen2.peek3Next();
    for (int i = 0; i < 3; i++) {
        assert(peeked[i] == gen2.next() && "peek ต้องตรงกับ next จริง");
    }
    std::cout << "[OK] peek3Next ตรงกับ next() ที่ตามมา\n";

    // 3. ดึงเยอะๆ ไม่ crash (คุมขนาด buffer ได้)
    for (int i = 0; i < 1000; i++) gen.next();
    std::cout << "[OK] ดึง 1000 ชิ้นไม่ crash\n";

    std::cout << "\nผ่านทุกเทส!\n";
}