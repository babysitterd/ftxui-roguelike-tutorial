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

    void Add(std::string const& message)
    {
        m_data.pop_front();
        m_data.push_back(message);
    }

    ftxui::Element Render() const
    {
        ftxui::Elements messages;
        messages.reserve(m_data.size());
        for (auto it = m_data.crbegin(); it != m_data.crend(); ++it)
        {
            messages.push_back(ftxui::text(*it) | ftxui::color(ftxui::Color::Aquamarine1));
        }

        return ftxui::vbox(messages);
    }

    std::list<std::string> m_data;
};
