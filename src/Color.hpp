#pragma once

#include <ftxui/dom/elements.hpp>

namespace Color
{

inline auto const BarEmpty = ftxui::Color(0x40, 0x10, 0x10);
inline auto const BarFilled = ftxui::Color(0x0, 0x60, 0x0);
inline auto const BarText = ftxui::Color::White;
inline auto const ControlsText = ftxui::Color::Yellow;
inline auto const DescentText = ftxui::Color(0x9F, 0x3F, 0xFF);
inline auto const EnemyAttack = ftxui::Color(0xFF, 0xC0, 0xC0);
inline auto const EnemyDie = ftxui::Color(0xFF, 0xA0, 0x30);
inline auto const HealthRecovered = ftxui::Color(0x0, 0xFF, 0x0);
inline auto const HintText = ftxui::Color::LightSlateGrey;
inline auto const PlayerAttack = ftxui::Color(0xE0, 0xE0, 0xE0);
inline auto const PlayerDie = ftxui::Color(0xFF, 0x30, 0x30);
inline auto const WelcomeText = ftxui::Color(0x20, 0xA0, 0xFF);

} // namespace Color
