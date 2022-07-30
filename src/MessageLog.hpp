#pragma once

#include <ftxui/dom/elements.hpp>

#include <string>
#include <vector>

struct MessageLog
{
    void Add(std::string const& message, ftxui::Color color)
    {
        m_data.emplace_back(message, color);
    }

    ftxui::Element RenderAll(std::size_t focused) const
    {
        ftxui::Elements messages;
        messages.reserve(m_data.size());
        for (auto it = m_data.cbegin(); it != m_data.cend(); ++it)
        {
            auto const& [text, color] = *it;
            messages.push_back(ftxui::text("  " + text) | ftxui::color(color));
        }

        auto const& [text, color] = m_data[focused];
        messages[focused] = ftxui::text("> " + text) | ftxui::color(color) | ftxui::focus;

        return ftxui::vbox(messages);
    }

    // Render last n messages
    ftxui::Element Render(std::size_t n) const
    {
        ftxui::Elements messages(n, ftxui::text(""));
        std::transform(m_data.crbegin(), m_data.crbegin() + std::min(n, m_data.size()),
                       messages.rbegin(), [](auto const& x) {
                           auto const& [text, color] = x;
                           return ftxui::text("  " + text) | ftxui::color(color) | ftxui::bold;
                       });

        // highlight last message
        auto const& [text, color] = m_data.back();
        messages.back() = ftxui::text("> " + text) | ftxui::color(color);

        return ftxui::vbox(messages);
    }

    std::vector<std::pair<std::string, ftxui::Color>> m_data;
};
