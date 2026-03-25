#include "handlers/dice_handler.h"

namespace erebus::handlers {

DiceHandler::DiceHandler(std::uint32_t seed) : rng_(seed) {}

int DiceHandler::rollOnce(erebus::domain::DiceType type) {
    std::uniform_int_distribution<int> dist(minValue(type), maxValue(type));
    return dist(rng_);
}

int DiceHandler::minValue(erebus::domain::DiceType type) {
    return (type == erebus::domain::DiceType::d100) ? 0 : 1;
}

int DiceHandler::maxValue(erebus::domain::DiceType type) {
    switch (type) {
        case erebus::domain::DiceType::d3:   return 3;
        case erebus::domain::DiceType::d4:   return 4;
        case erebus::domain::DiceType::d6:   return 6;
        case erebus::domain::DiceType::d8:   return 8;
        case erebus::domain::DiceType::d10:  return 10;
        case erebus::domain::DiceType::d12:  return 12;
        case erebus::domain::DiceType::d100: return 99;
    }
    return 1; // unreachable — satisfaz -Wreturn-type
}

} // namespace erebus::handlers
