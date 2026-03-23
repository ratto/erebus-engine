#pragma once
#include "ports/i_event_bus.h"

namespace erebus::adapters {

// Implementação inicial do EventBus: imprime eventos no terminal (stdout).
// Será expandida em SPECs futuras para suportar subscribers dinâmicos
// e integração com SSE/WebSocket.
class EventBusImpl : public erebus::ports::IEventBus {
public:
    void publish(const erebus::events::DiceRolledEvent& event) override;
};

} // namespace erebus::adapters
