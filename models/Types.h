// Types.h
#pragma once

// ชนิดของ tetromino — ใช้เป็นทั้ง "ชนิดชิ้น" และ "สีของ cell"
// Empty = ช่องว่าง (ยังไม่มีบล็อก)
enum class PieceType {
    Empty,
    I, O, T, S, Z, J, L
};

struct Point { int x, y; };