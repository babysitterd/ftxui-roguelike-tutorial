#include <ftxui/screen/color.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/component.hpp>

namespace
{

constexpr int screen_width = 80;
constexpr int screen_height = 50;

}  // namespace

struct Point
{
    int x = 0;
    int y = 0;
};

bool operator==(Point const& lhs, Point const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

class Actor
{
public :
   Actor(Point const& point, char codepoint, ftxui::Color const& color);
   ftxui::Element render() const;

   Point m_point;
   char m_codepoint;
   ftxui::Color m_color;
};

Actor::Actor(Point const& point, char codepoint, ftxui::Color const& color) :
    m_point(point), m_codepoint(codepoint), m_color(color)
{
}

ftxui::Element Actor::render() const
{
    return ftxui::text(std::string{m_codepoint}) | ftxui::color(m_color);
}

struct World
{
    explicit World(Point const& player_position);

    ftxui::Element render() const;

    Actor m_player;
};

World::World(Point const& player_position) :
    m_player(player_position, '@', ftxui::Color::Yellow)
{
}

ftxui::Element World::render() const
{
    ftxui::Elements world;
    world.reserve(screen_height);
    for (int i = 0; i < screen_height; ++i)
    {
        ftxui::Elements row;
        row.reserve(screen_width);
        for (int j = 0; j < screen_width; ++j)
        {
            if (Point{j, i} == m_player.m_point)
            {
                row.push_back(m_player.render());
            }
            else
            {
                row.push_back(ftxui::text("."));
            }
        }
        world.push_back(ftxui::hbox(row));
    }
    return ftxui::vbox(world);
}

int main()
{
    using namespace ftxui;

    World world({40, 25});

    auto screen = ScreenInteractive::FitComponent();
    auto renderer = Renderer([&world] {
        return border(world.render());
    });

    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }

        if (event == ftxui::Event::ArrowUp)
        {
            --world.m_player.m_point.y;
            return true;
        }

        if (event == ftxui::Event::ArrowRight)
        {
            ++world.m_player.m_point.x;
            return true;
        }

        if (event == ftxui::Event::ArrowDown)
        {
            ++world.m_player.m_point.y;
            return true;
        }

        if (event == ftxui::Event::ArrowLeft)
        {
            --world.m_player.m_point.x;
            return true;
        }

        return false;
    });

    screen.Loop(component);
}
