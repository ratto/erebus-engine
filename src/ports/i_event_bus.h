#pragma once
#include "events/dice_events.h"

namespace erebus::ports {

class IEventBus {
public:
    virtual ~IEventBus() = default;

    virtual void publish(const erebus::events::DiceRolledEvent& event) = 0;

    // Novos overloads de publish serão adicionados conforme novas features
    // forem implementadas (SkillTestedEvent, CombatResolvedEvent etc.)
};

} // namespace erebus::ports
