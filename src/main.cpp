#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

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

class Actor
{
public :
   Actor(Point const& point, char codepoint, ftxui::Color const& color);
   ftxui::Element render() const;
   int x() const;
   int y() const;

private:
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

int Actor::x() const
{
    return m_point.x;
}

int Actor::y() const
{
    return m_point.y;
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
            if (i == m_player.x() && j == m_player.y())
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

    Element document = border(world.render());

    auto screen = Screen::Create(
      Dimension::Fit(document),  // Width
      Dimension::Fit(document)   // Height
    );
    Render(screen, document);
    screen.Print();
}
