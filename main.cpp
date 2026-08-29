#include "models/Game.h"
#include "handlers/Renderer.h"
#include <ftxui/screen/screen.hpp>      // Screen, Dimension
#include <ftxui/dom/node.hpp>           // Render
#include <iostream>

int main() {
    Game game;
    Renderer renderer;
    
    game.hardDrop();
    game.hardDrop();
    game.hold();

    ftxui::Element document = ftxui::hbox({
        ftxui::vbox({
            renderer.renderHold(game),
            renderer.renderStats(game)
        }),
        renderer.renderBoard(game),
        renderer.renderNext(game)
    });

    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Fit(document)    // ขนาดพอดีกับ element
    );
    ftxui::Render(screen, document);       // วาด element ลง screen
    screen.Print();                        // พ่นออก terminal
    std::cout << std::endl;                // ขึ้นบรรทัดใหม่ให้ prompt ไม่ทับ
    return 0;
}