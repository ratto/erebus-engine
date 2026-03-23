#pragma once
#include "ports/i_dice_roller.h"
#include "ports/i_event_bus.h"
#include "handlers/dice_handler.h"

namespace erebus::services {

class DiceService : public erebus::ports::IDiceRoller {
public:
    // IEventBus é injetado para permitir substituição em testes.
    explicit DiceService(
        erebus::ports::IEventBus& eventBus,
        erebus::handlers::DiceHandler handler = erebus::handlers::DiceHandler{}
    );

    erebus::domain::DiceRoll roll(
        erebus::domain::DiceType type,
        int count = 1
    ) override;

private:
    erebus::ports::IEventBus& eventBus_;
    erebus::handlers::DiceHandler handler_;
};

} // namespace erebus::services
