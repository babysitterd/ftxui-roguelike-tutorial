#include <ftxui/screen/color.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/component.hpp>

#include <set>
#include <vector>

namespace
{

constexpr int screen_width = 80;
constexpr int screen_height = 50;

}  // namespace

struct Point
{
    int x = 0;
    int y = 0;
    // Whoaa SPACESHIP is here!
    auto operator<=>(Point const&) const = default;
};

class Actor
{
public :
   Actor(Point const& point, char codepoint, ftxui::Color const& color);
   ftxui::Element render() const;

   Point m_point;
   char m_codepoint;
   ftxui::Color m_color;
};

// allows look up std::set by Point
struct Compare
{
    using is_transparent = void;

    bool operator()(Actor const& lhs, Actor const& rhs) const
    {
        return lhs.m_point < rhs.m_point;
    }

    bool operator()(Point const& lhs, Actor const& rhs) const
    {
        return lhs < rhs.m_point;
    }

    bool operator()(Actor const& lhs, Point const& rhs) const
    {
        return lhs.m_point < rhs;
    }
};

Actor::Actor(Point const& point, char codepoint, ftxui::Color const& color) :
    m_point(point), m_codepoint(codepoint), m_color(color)
{
}

ftxui::Element Actor::render() const
{
    return ftxui::text(std::string{m_codepoint}) | ftxui::color(m_color);
}

struct Tile {
   bool canWalk; // can we walk through this tile?
   Tile() : canWalk(true) {}
};

class Map {
public :
   Map(int width, int height);

   bool isWall(Point const& point) const;
   void setWall(Point const& point);

   ftxui::Element render(Point const& point) const;

   int m_width;
   int m_height;
   std::vector<Tile> m_tiles;
};

Map::Map(int width, int height) : m_width(width), m_height(height), m_tiles(m_width * m_height)
{
    setWall({30, 22});
    setWall({50, 22});
}

bool Map::isWall(Point const& point) const
{
    return !m_tiles[point.x + point.y * m_width].canWalk;
}

void Map::setWall(Point const& point) {
   m_tiles[point.x + point.y * m_width].canWalk = false;
}

ftxui::Element Map::render(Point const& point) const
{
    static const ftxui::Color darkWall = ftxui::Color::CadetBlue;
    static const ftxui::Color darkGround(50,50,150);

    if (isWall(point)) {
        return ftxui::text(std::string{'#'}) | ftxui::color(darkWall);
    } else {
        return ftxui::text(std::string{'.'}) | ftxui::color(darkGround);
    }
}

struct World
{
    explicit World(Point const& player_position);

    ftxui::Element render() const;

    Actor m_player;
    Map m_map;
    std::set<Actor, Compare> m_actors;
};

World::World(Point const& player_position) :
    m_player(player_position, '@', ftxui::Color::White),
    m_map(screen_width, screen_height)
{
    m_actors.emplace(Point{60, 13}, '@', ftxui::Color::Yellow);
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
            Point const current{j, i};
            if (current == m_player.m_point)
            {
                row.push_back(m_player.render());
            }
            else
            {
                auto it = m_actors.find(current);
                if (it == m_actors.end())
                {
                    row.push_back(m_map.render(current));
                }
                else
                {
                    row.push_back(it->render());
                }
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

        auto rewind = [&world, previous = world.m_player.m_point]()
        {
            if (world.m_map.isWall(world.m_player.m_point))
            {
                world.m_player.m_point = previous;
            }
        };

        if (event == ftxui::Event::ArrowUp)
        {
            --world.m_player.m_point.y;
            rewind();
            return true;
        }

        if (event == ftxui::Event::ArrowRight)
        {
            ++world.m_player.m_point.x;
            rewind();
            return true;
        }

        if (event == ftxui::Event::ArrowDown)
        {
            ++world.m_player.m_point.y;
            rewind();
            return true;
        }

        if (event == ftxui::Event::ArrowLeft)
        {
            --world.m_player.m_point.x;
            rewind();
            return true;
        }

        return false;
    });

    screen.Loop(component);
}
