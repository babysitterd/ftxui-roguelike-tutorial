#pragma once

#include <random>

class RNG
{
  public:
    explicit RNG(unsigned int seed = std::random_device{}()) : m_generator(seed)
    {
    }

    int RandomInt(int from, int to)
    {
        std::uniform_int_distribution<> distribution(from, to);
        return distribution(m_generator);
    }

  private:
    std::mt19937 m_generator;
};
