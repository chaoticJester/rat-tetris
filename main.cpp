#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

int main() {
    std::string name;
    auto input = Input(&name, "พิมพ์ชื่อ...");

    std::vector<std::string> options = {"ตัวเลือก 1", "ตัวเลือก 2", "ตัวเลือก 3"};
    int selected = 0;
    auto menu = Menu(&options, &selected);

    auto layout = Container::Vertical({input, menu});

    auto renderer = Renderer(layout, [&] {
        return vbox({
            text("ชื่อ: " + name),
            separator(),
            input->Render(),
            separator(),
            menu->Render(),
        }) | border;
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
}