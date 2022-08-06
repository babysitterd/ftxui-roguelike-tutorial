#include <ftxui/component/component.hpp> // Renderer
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "Point.hpp"
#include "World.hpp"

int main()
{
    using namespace ftxui;

    // web vesrion of the terminal window by default is 43 lines only
    int const mapWidth = 110;
    int const mapHeight = 37;
    int const fovRadius = 10;

    World world(mapWidth, mapHeight, fovRadius);

    auto screen = ScreenInteractive::FitComponent();
    auto renderer = Renderer([&world] { return world.Render(); });

    auto component = CatchEvent(renderer, [&world, &screen](Event event) {
        if (event == Event::Character('q'))
        {
            screen.ExitLoopClosure()();
            return true;
        }

        return world.EventHandler(event);
    });

    screen.Loop(component);
}
