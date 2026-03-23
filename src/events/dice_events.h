#pragma once
#include "domain/dice.h"

namespace erebus::events {

struct DiceRolledEvent {
    erebus::domain::DiceType diceType;
    int result;
    int rollIndex;
    int totalCount;
};

} // namespace erebus::events
