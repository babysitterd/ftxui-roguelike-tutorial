#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>  // Renderer
#include <ftxui/component/event.hpp>

#include "Point.hpp"
#include "World.hpp"

int main()
{
    using namespace ftxui;

    Point const playerStartingPosition{40, 25};
    int const mapWidth = 80;
    int const mapHeight = 50;
    World world(playerStartingPosition, mapWidth, mapHeight);

    auto screen = ScreenInteractive::FitComponent();
    auto renderer = Renderer([&world] {
        return border(world.render());
    });

    auto component = CatchEvent(renderer, [&world, &screen](Event event) {
        if (event == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }

        return world.EventHandler(event);
    });

    screen.Loop(component);
}
