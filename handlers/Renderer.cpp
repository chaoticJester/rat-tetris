#include "Renderer.h"

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

