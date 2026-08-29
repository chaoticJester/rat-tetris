// handlers/Renderer.h
#pragma once

#include "Types.h"
#include "../models/Game.h"
#include <string>
#include <ftxui/dom/elements.hpp>
#include <array>

class Renderer {    
    private:
        ftxui::Element renderPieceBox(PieceType p);
    public:
    // ชิ้น pure logic — เทสได้ไม่ต้องมี FTXUI
        std::array<std::array<GameColor, 10>, 20> buildRenderGrid(const Game& game);
        ftxui::Element renderBoard(const Game& game);
        ftxui::Element renderStats(const Game& game);
        ftxui::Element renderHold(const Game& game);
        ftxui::Element renderNext(const Game& game);
};