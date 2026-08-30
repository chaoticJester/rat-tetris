#include "Renderer.h"

Color Renderer::toRaylibColor(GameColor c) {
    switch (c) {
        case GameColor::Cyan:   return SKYBLUE;    // หรือ Color{0,255,255,255}
        case GameColor::Yellow: return YELLOW;
        case GameColor::Purple: return PURPLE;
        case GameColor::Green:  return GREEN;
        case GameColor::Red:    return RED;
        case GameColor::Blue:   return BLUE;
        case GameColor::Orange: return ORANGE;
        case GameColor::Ghost:  return Color{80, 80, 80, 255};   // เทาจาง
        case GameColor::Empty:  return Color{20, 20, 20, 255};   // พื้นกระดานเข้ม
    }
    return BLACK;
}

std::array<std::array<GameColor, 10>, 22> Renderer::buildRenderGrid(const Game& game) {
    std::array<std::array<GameColor, 10>, 22> grid = {GameColor::Empty};

    const Board& board = game.getBoard();   
    for(int x = 0; x < 10; x++) {
        for(int y = 18; y < 40; y++) {
            Cell cell = board.getCell(x, y);
            if(!cell.isEmpty()) {
                grid[y - 18][x] = pieceToColor(cell.type);
            }
        }
    }

    for (auto& block : game.getGhostBlocks()) {
        if (block.y >= 18) {                    // อยู่ในโซนที่เห็นเท่านั้น
            grid[block.y - 18][block.x] = GameColor::Ghost;
        }
    }


    for (auto& block : game.getCurrentBlocks()) {
        if (block.y >= 18) {                    // อยู่ในโซนที่เห็นเท่านั้น
            grid[block.y - 18][block.x] = pieceToColor(game.getCurrentType());
        }
    }
    return grid;
}

void Renderer::renderBoard(const Game& game) {
    // วาดทดสอบ: สี่เหลี่ยมมุมซ้ายบน
    const int CELL = 30;        // ขนาดบล็อก 30×30 pixel
    const int OX = 120, OY = 18;  // จุดเริ่มวาด board (offset ซ้าย/บน)

    auto grid = buildRenderGrid(game);
    for (int y = 0; y < 22; y++) {
        for (int x = 0; x < 10; x++) {
            Color col = toRaylibColor(grid[y][x]);
            if(y < 2 && grid[y][x] == GameColor::Empty) {
                col = Color{BLACK};
            }
            DrawRectangle(OX + x*CELL, OY + y*CELL, CELL, CELL, col);
            if (y >= 2) {
                DrawRectangleLines(OX + x*CELL, OY + y*CELL, CELL, CELL, Color{40,40,40,255});
            }
        }
    }
}

void Renderer::renderStats(const Game& game) {
    int sx = 10, sy = 250;   // ตำแหน่ง
    DrawText(TextFormat("LEVEL: %d", game.getLevel()), sx, sy, 20, RAYWHITE);
    DrawText(TextFormat("LINES: %d", game.getTotalLines()), sx, sy + 30, 20, RAYWHITE);
    DrawText("SCORE:", sx, sy + 60, 20, RAYWHITE);
    DrawText(TextFormat("%d", game.getScore()), sx, sy + 90, 20, RAYWHITE);
}

void Renderer::drawPieceAt(PieceType type, int px, int py) {
    if (type == PieceType::Empty) return;
    const int CELL = 25;   // ขนาดบล็อกในกล่องเล็ก (เล็กกว่า board นิดหน่อยก็ได้)
    Tetromino piece(type);
    for (auto& b : piece.getLocalBlocks()) {
        Color col = toRaylibColor(pieceToColor(type));
        DrawRectangle(px + b.x*CELL, py + b.y*CELL, CELL, CELL, col);
        DrawRectangleLines(px + b.x*CELL, py + b.y*CELL, CELL, CELL, Color{40,40,40,255});
    }
}

void Renderer::renderHold(const Game& game) {
    DrawText("HOLD", 10, 10, 20, RAYWHITE);
    DrawRectangleLines(10, 35, 4*25, 4*25, RAYWHITE);   // กรอบกล่อง
    drawPieceAt(game.getHoldType(), 10, 35);
}

void Renderer::renderNext(const Game& game) {
    int nx = 500, ny = 35;   // ตำแหน่งขวา
    DrawText("NEXT", nx, 10, 20, RAYWHITE);
    auto next5 = game.getNext5Pieces();
    for (int i = 0; i < 5; i++) {
        drawPieceAt(next5[i], nx, ny + i * (4*25 + 10));   // เว้นระยะแต่ละชิ้น
    }
}

void Renderer::render(const Game& game) {
    renderBoard(game); 
    renderHold(game);
    renderNext(game);
    renderStats(game);
}