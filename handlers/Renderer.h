// handlers/Renderer.h
#pragma once

#include "Types.h"
#include "raylib.h"
#include "../models/Game.h"
#include <string>
#include <array>

class Renderer {    
    private:
        Color toRaylibColor(GameColor c);
        std::array<std::array<GameColor, 10>, 22> buildRenderGrid(const Game& game);
        void drawPieceAt(PieceType type, int px, int py);
    public:
        void renderBoard(const Game& game);
        void renderStats(const Game& game);
        void renderHold(const Game& game);
        void renderNext(const Game& game);
        void render(const Game& game);
};