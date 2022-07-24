#pragma once

#include <ftxui/dom/elements.hpp>

#include <list>
#include <string>

// simple circular buffer
struct MessageLog
{
    static constexpr int MaxCapacity = 5;

    MessageLog() : m_data(MaxCapacity)
    {
    }

    void Add(std::string const& message, ftxui::Color color)
    {
        m_data.pop_front();
        m_data.emplace_back(message, color);
    }

    ftxui::Element Render() const
    {
        ftxui::Elements messages;
        messages.reserve(m_data.size());
        for (auto it = m_data.crbegin(); it != m_data.crend(); ++it)
        {
            auto const& [text, color] = *it;
            messages.push_back(ftxui::text(text) | ftxui::color(color));
        }

        return ftxui::vbox(messages);
    }

    std::list<std::pair<std::string, ftxui::Color>> m_data;
};
