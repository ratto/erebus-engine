#include "adapters/event_bus_impl.h"
#include <iostream>

namespace {

const char* diceTypeName(erebus::domain::DiceType type) {
    switch (type) {
        case erebus::domain::DiceType::d3:   return "d3";
        case erebus::domain::DiceType::d4:   return "d4";
        case erebus::domain::DiceType::d6:   return "d6";
        case erebus::domain::DiceType::d8:   return "d8";
        case erebus::domain::DiceType::d10:  return "d10";
        case erebus::domain::DiceType::d12:  return "d12";
        case erebus::domain::DiceType::d100: return "d100";
    }
    return "unknown"; // unreachable — satisfaz -Wreturn-type
}

} // anonymous namespace

namespace erebus::adapters {

void EventBusImpl::publish(const erebus::events::DiceRolledEvent& event) {
    std::cerr
        << "[DiceRolled] "
        << diceTypeName(event.diceType)
        << " roll " << event.rollIndex << "/" << event.totalCount
        << " \xe2\x86\x92 " << event.result
        << "\n";
}

} // namespace erebus::adapters
