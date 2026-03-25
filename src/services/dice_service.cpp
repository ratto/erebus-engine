#include "services/dice_service.h"
#include "events/dice_events.h"
#include <stdexcept>

namespace erebus::services {

DiceService::DiceService(
    erebus::ports::IEventBus& eventBus,
    erebus::handlers::DiceHandler handler)
    : eventBus_(eventBus), handler_(std::move(handler)) {}

erebus::domain::DiceRoll DiceService::roll(
    erebus::domain::DiceType type,
    int count)
{
    if (count <= 0) {
        throw std::invalid_argument(
            "count must be a positive integer greater than 0");
    }

    erebus::domain::DiceRoll result;
    result.results.reserve(static_cast<std::size_t>(count));

    for (int i = 0; i < count; ++i) {
        int value = handler_.rollOnce(type);
        result.results.push_back(value);

        erebus::events::DiceRolledEvent event{
            type,
            value,
            i + 1,
            count
        };
        eventBus_.publish(event);
    }

    return result;
}

} // namespace erebus::services
