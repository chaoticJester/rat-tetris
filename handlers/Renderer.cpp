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

void Renderer::drawPieceAt(PieceType type, int px, int py, bool justSwap) {
    if (type == PieceType::Empty) return;
    const int CELL = 25;   // ขนาดบล็อกในกล่องเล็ก

    Tetromino piece(type);
    auto blocks = piece.getLocalBlocks();

    // 1. หาจุดต่ำสุด-สูงสุด ของชิ้นส่วน (Bounding Box จริงๆ ของรูปทรง)
    int minX = 4, maxX = -1;
    int minY = 4, maxY = -1;
    for (const auto& b : blocks) {
        if (b.x < minX) minX = b.x;
        if (b.x > maxX) maxX = b.x;
        if (b.y < minY) minY = b.y;
        if (b.y > maxY) maxY = b.y;
    }

    // 2. คำนวณความกว้าง และความสูง ของชิ้นส่วน (หน่วยเป็นจำนวนบล็อก)
    int pieceWidthBlocks = (maxX - minX) + 1;
    int pieceHeightBlocks = (maxY - minY) + 1;

    // 3. แปลงเป็นพิกเซล
    int pieceWidthPx = pieceWidthBlocks * CELL;
    int pieceHeightPx = pieceHeightBlocks * CELL;

    // 4. ขนาดของกล่อง (จากโค้ดเดิมของคุณคือ 4*25 = 100)
    int boxSize = 4 * CELL;

    // 5. คำนวณ Offset ให้ชิ้นส่วนอยู่กึ่งกลางกล่อง
    // (ขนาดกล่อง - ขนาดชิ้นส่วน) / 2
    int offsetX = (boxSize - pieceWidthPx) / 2;
    int offsetY = (boxSize - pieceHeightPx) / 2;

    // 6. วาดชิ้นส่วน โดยหักลบตำแหน่ง minX, minY ด้วย เพื่อให้ขอบซ้าย/บนของชิ้นส่วน เริ่มที่ 0
    for (const auto& b : blocks) {
        Color col = toRaylibColor((justSwap)? GameColor::Ghost: pieceToColor(type));
        
        int drawX = px + offsetX + ((b.x - minX) * CELL);
        int drawY = py + offsetY + ((b.y - minY) * CELL);

        DrawRectangle(drawX, drawY, CELL, CELL, col);
        DrawRectangleLines(drawX, drawY, CELL, CELL, Color{40,40,40,255});
    }
}

void Renderer::renderBoard(const Game& game) {
    const int CELL = 30;          // ขนาดบล็อก 30x30 pixel
    const int OX = 490, OY = 30;  // เปลี่ยนจุดเริ่มวาดให้อยู่ตรงกลางหน้าจอ

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
    int sx = 320, sy = 250;   
    DrawText(TextFormat("LEVEL: %d", game.getLevel()), sx, sy, 20, RAYWHITE);
    DrawText("LINES:", sx, sy + 30, 20, RAYWHITE);
    DrawText(TextFormat("%d", game.getTotalLines()), sx, sy + 60, 20, RAYWHITE); // ขยับมาที่ sy + 60
    DrawText("SCORE:", sx, sy + 90, 20, RAYWHITE);                               // ขยับมาที่ sy + 90
    DrawText(TextFormat("%d", game.getScore()), sx, sy + 120, 20, RAYWHITE);     // ขยับมาที่ sy + 120
}

void Renderer::renderHold(const Game& game, bool justSwap) {
    int hx = 320, hy = 30;    // ตำแหน่งซ้ายของกระดาน
    DrawText("HOLD", hx, hy, 20, RAYWHITE);
    DrawRectangleLines(hx, hy + 25, 4*25, 4*25, RAYWHITE);   // เลื่อนกรอบลงมาจากข้อความ
    drawPieceAt(game.getHoldType(), hx, hy + 25, justSwap);
}

void Renderer::renderNext(const Game& game) {
    int nx = 830, ny = 55;    // ตำแหน่งขวาของกระดาน (490 + 300 + 40)
    DrawText("NEXT", nx, 30, 20, RAYWHITE);
    auto next5 = game.getNext5Pieces();
    for (int i = 0; i < 5; i++) {
        drawPieceAt(next5[i], nx, ny + i * (4*25 + 10), false);   
    }
}
void Renderer::render(const Game& game, bool justSwap) {
    renderBoard(game); 
    renderHold(game, justSwap);
    renderNext(game);
    renderStats(game);
}