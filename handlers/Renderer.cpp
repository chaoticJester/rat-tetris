#include "Renderer.h"

namespace {
    ftxui::Color toFtxuiColor(GameColor c) {
        switch(c) {
        case GameColor::Blue:
            return ftxui::Color::Blue;
        case GameColor::Cyan:
            return ftxui::Color::Cyan1;
        case GameColor::Green:
            return ftxui::Color::GreenLight;
        case GameColor::Orange:
            return ftxui::Color::DarkOrange3;
        case GameColor::Magenta:
            return ftxui::Color::Magenta;
        case GameColor::Red:
            return ftxui::Color::Red;
        case GameColor::Yellow:
            return ftxui::Color::Yellow;
        case GameColor::Ghost:
            return ftxui::Color::Grey35;
        default:
            return ftxui::Color::Grey3;
        };
    }   
}

std::array<std::array<GameColor, 10>, 20> Renderer::buildRenderGrid(const Game& game) {
    std::array<std::array<GameColor, 10>, 20> grid = {GameColor::Empty};

    const Board& board = game.getBoard();   
    for(int x = 0; x < 10; x++) {
        for(int y = 20; y < 40; y++) {
            Cell cell = board.getCell(x, y);
            if(!cell.isEmpty()) {
                grid[y - 20][x] = pieceToColor(cell.type);
            }
        }
    }

    for (auto& block : game.getGhostBlocks()) {
        if (block.y >= 20) {                    // อยู่ในโซนที่เห็นเท่านั้น
            grid[block.y - 20][block.x] = GameColor::Ghost;
        }
    }


    for (auto& block : game.getCurrentBlocks()) {
        if (block.y >= 20) {                    // อยู่ในโซนที่เห็นเท่านั้น
            grid[block.y - 20][block.x] = pieceToColor(game.getCurrentType());
        }
    }
    return grid;
}

ftxui::Element Renderer::renderBoard(const Game& game) {
    auto grid = buildRenderGrid(game);
    
    ftxui::Elements rows;            // = std::vector<Element>
    for (int y = 0; y < 20; y++) {
        ftxui::Elements cells;
        for (int x = 0; x < 10; x++) {
            cells.push_back(ftxui::text("██") | color(toFtxuiColor(grid[y][x])));
        }
        rows.push_back(hbox(cells));
    }
    return vbox(rows) | ftxui::border;
}

ftxui::Element Renderer::renderStats(const Game& game) {
    return ftxui::vbox({
        ftxui::text("LEVEL: " + std::to_string(game.getLevel())),
        ftxui::text("TOTAL LINE: "  + std::to_string(game.getTotalLines())),
        ftxui::text(""),                                              // เว้นบรรทัด
        ftxui::text("SCORES:"),
        ftxui::text(std::to_string(game.getScore())),
    });
}

ftxui::Element Renderer::renderPieceBox(PieceType p) {
    std::array<std::array<GameColor, 4>, 4> holdGrid = {GameColor::Empty};
    
    Tetromino hold = Tetromino(p);
    for(auto& blocks : hold.getLocalBlocks()) {
        if (blocks.y >= 0 && blocks.y < 4 && blocks.x >= 0 && blocks.x < 4) {
            holdGrid[blocks.y][blocks.x] = pieceToColor(hold.getPieceType());
        }
    }

    ftxui::Elements rows;            // = std::vector<Element>
    for (int y = 0; y < 4; y++) {
        ftxui::Elements cells;
        for (int x = 0; x < 4; x++) {
            cells.push_back(ftxui::text("██") | color(toFtxuiColor(holdGrid[y][x])));
        }
        rows.push_back(hbox(cells));
    }
    return ftxui::vbox(rows);
}

ftxui::Element Renderer::renderHold(const Game& game) {
    return renderPieceBox(game.getHoldType()) | ftxui::border;
}

ftxui::Element Renderer::renderNext(const Game& game) {
    std::array<PieceType, 5> next5 = game.getNext5Pieces();

    ftxui::Elements pieceBoxes;
    for (PieceType p : next5) {
        pieceBoxes.push_back(renderPieceBox(p));      // 1 ชิ้น = 1 กล่อง
    }
    // รวม 5 กล่อง คั่นด้วย separato
    return ftxui::vbox(pieceBoxes) | ftxui::border;
 }