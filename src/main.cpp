#include <ftxui/component/component.hpp> // Renderer
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "Point.hpp"
#include "World.hpp"

int main()
{
    using namespace ftxui;

    int const mapWidth = 80;
    int const mapHeight = 45;
    int const fovRadius = 10;
    World world(mapWidth, mapHeight, fovRadius);

    auto screen = ScreenInteractive::FitComponent();
    auto renderer = Renderer([&world] { return border(world.Render()); });

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
