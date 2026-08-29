// Types.h
#pragma once

// ชนิดของ tetromino — ใช้เป็นทั้ง "ชนิดชิ้น" และ "สีของ cell"
// Empty = ช่องว่าง (ยังไม่มีบล็อก)
enum class PieceType {
    Empty,
    I, O, T, S, Z, J, L
};

struct Point { int x, y; };

enum class GameColor { 
    Empty, 
    Cyan, Yellow, Purple, Green, Red, Blue, Orange, 
    Ghost
};

static const GameColor COLOR_MAP[] = {
    GameColor::Empty, 
    GameColor::Cyan, GameColor::Yellow, GameColor::Purple, GameColor::Green, GameColor::Red, GameColor::Blue, GameColor::Orange,
    GameColor::Ghost
};

inline GameColor pieceToColor(PieceType t) { return COLOR_MAP[(int)t]; }