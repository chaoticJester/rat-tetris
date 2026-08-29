// handlers/Renderer.h
#pragma once
#include "Types.h"
#include "../models/Game.h"
#include <array>

class Renderer {    
    public:
    // ชิ้น pure logic — เทสได้ไม่ต้องมี FTXUI
        std::array<std::array<GameColor, 10>, 20> buildRenderGrid(const Game& game);
    // (ทีหลัง) ชิ้นที่พึ่ง FTXUI — คืน ftxui::Element
    // ftxui::Element render(const Game& game) const;
};