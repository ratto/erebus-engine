#pragma once
#include <vector>

namespace erebus::domain {

enum class DiceType { d3, d4, d6, d8, d10, d12, d100 };

struct DiceRoll {
    std::vector<int> results;
    int total() const;
};

} // namespace erebus::domain
