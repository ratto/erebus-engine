#pragma once
#include "domain/dice.h"

namespace erebus::ports {

class IDiceRoller {
public:
    virtual ~IDiceRoller() = default;

    // Rola `count` vezes o dado `type`.
    // Pré-condição: count > 0. Lança std::invalid_argument se violada.
    virtual erebus::domain::DiceRoll roll(
        erebus::domain::DiceType type,
        int count = 1
    ) = 0;
};

} // namespace erebus::ports
