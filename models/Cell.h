// Cell.h
#pragma once

#include "Types.h"   // ต้อง include เต็ม เพราะเก็บ PieceType เป็น value member

// Cell = 1 ช่องในกริดของ Board
// ใช้ struct เพราะเป็นแค่ที่เก็บข้อมูล ไม่มี logic ซับซ้อน (member เป็น public โดย default)
struct Cell {
    // ชนิด/สีของช่องนี้ ค่าเริ่มต้นคือ Empty (ช่องว่าง)
    PieceType type = PieceType::Empty;

    // helper เล็กๆ ถามว่าช่องนี้ว่างไหม
    // เขียน body ใน header ได้เลยเพราะสั้นมาก (implicit inline — ไม่เกิด multiple definition)
    bool isEmpty() const {
        return type == PieceType::Empty;
    }
};