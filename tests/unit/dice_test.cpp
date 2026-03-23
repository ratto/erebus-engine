#include <gtest/gtest.h>
#include <stdexcept>
#include "domain/dice.h"
#include "ports/i_dice_roller.h"
#include "ports/i_event_bus.h"
#include "events/dice_events.h"
#include "handlers/dice_handler.h"
#include "services/dice_service.h"

using namespace erebus::domain;
using namespace erebus::events;
using namespace erebus::handlers;
using namespace erebus::services;

// ─── Test Doubles ─────────────────────────────────────────────────────────────

class SpyEventBus : public erebus::ports::IEventBus {
public:
    std::vector<DiceRolledEvent> capturedEvents;

    void publish(const DiceRolledEvent& event) override {
        capturedEvents.push_back(event);
    }
};

// ─── DiceRoll::total() ────────────────────────────────────────────────────────

TEST(DiceRollTotal, VetorVazio_EZero) {
    DiceRoll roll;
    EXPECT_EQ(roll.total(), 0);
}

TEST(DiceRollTotal, UmElemento_EIgualAoElemento) {
    DiceRoll roll;
    roll.results = {7};
    EXPECT_EQ(roll.total(), 7);
}

TEST(DiceRollTotal, TresElementos_EhASoma) {
    DiceRoll roll;
    roll.results = {3, 5, 2};
    EXPECT_EQ(roll.total(), 10);
}

// ─── DiceHandler: intervalos por tipo de dado ─────────────────────────────────

static void assertHandlerRange(DiceType type, int min, int max, int iterations = 500) {
    DiceHandler handler(42);
    for (int i = 0; i < iterations; ++i) {
        int result = handler.rollOnce(type);
        EXPECT_GE(result, min);
        EXPECT_LE(result, max);
    }
}

TEST(DiceHandler, D3_RetornaEntre1e3)    { assertHandlerRange(DiceType::d3,   1, 3);  }
TEST(DiceHandler, D4_RetornaEntre1e4)    { assertHandlerRange(DiceType::d4,   1, 4);  }
TEST(DiceHandler, D6_RetornaEntre1e6)    { assertHandlerRange(DiceType::d6,   1, 6);  }
TEST(DiceHandler, D8_RetornaEntre1e8)    { assertHandlerRange(DiceType::d8,   1, 8);  }
TEST(DiceHandler, D10_RetornaEntre1e10)  { assertHandlerRange(DiceType::d10,  1, 10); }
TEST(DiceHandler, D12_RetornaEntre1e12)  { assertHandlerRange(DiceType::d12,  1, 12); }
TEST(DiceHandler, D100_RetornaEntre0e99) { assertHandlerRange(DiceType::d100, 0, 99); }

// ─── DiceService: contagem de resultados ─────────────────────────────────────

TEST(DiceService, CountOmitido_Retorna1Resultado) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    DiceRoll result = service.roll(DiceType::d6);
    EXPECT_EQ(result.results.size(), 1u);
}

TEST(DiceService, Count3_Retorna3Resultados) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    DiceRoll result = service.roll(DiceType::d8, 3);
    EXPECT_EQ(result.results.size(), 3u);
}

TEST(DiceService, TotalConsistente) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    DiceRoll result = service.roll(DiceType::d6, 4);
    int expected = 0;
    for (int v : result.results) expected += v;
    EXPECT_EQ(result.total(), expected);
}

// ─── DiceService: eventos publicados ─────────────────────────────────────────

TEST(DiceService, PublicaUmEventoPorRolagem) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    service.roll(DiceType::d6, 3);
    EXPECT_EQ(bus.capturedEvents.size(), 3u);
}

TEST(DiceService, EventoContemRollIndexCorreto) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    service.roll(DiceType::d4, 3);
    EXPECT_EQ(bus.capturedEvents[0].rollIndex, 1);
    EXPECT_EQ(bus.capturedEvents[1].rollIndex, 2);
    EXPECT_EQ(bus.capturedEvents[2].rollIndex, 3);
}

TEST(DiceService, EventoContemTotalCountCorreto) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    service.roll(DiceType::d6, 5);
    for (const auto& event : bus.capturedEvents) {
        EXPECT_EQ(event.totalCount, 5);
    }
}

TEST(DiceService, EventoContemDiceTypeCorreto) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    service.roll(DiceType::d12, 2);
    EXPECT_EQ(bus.capturedEvents[0].diceType, DiceType::d12);
    EXPECT_EQ(bus.capturedEvents[1].diceType, DiceType::d12);
}

TEST(DiceService, ResultadoNoEventoConsistenteComDiceRoll) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    DiceRoll result = service.roll(DiceType::d8, 2);
    EXPECT_EQ(bus.capturedEvents[0].result, result.results[0]);
    EXPECT_EQ(bus.capturedEvents[1].result, result.results[1]);
}

// ─── DiceService: validação de count ─────────────────────────────────────────

TEST(DiceService, Count0_LancaInvalidArgument) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    EXPECT_THROW(service.roll(DiceType::d6, 0), std::invalid_argument);
}

TEST(DiceService, CountNegativo_LancaInvalidArgument) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    EXPECT_THROW(service.roll(DiceType::d6, -1), std::invalid_argument);
}

TEST(DiceService, CountInvalido_NaoPublicaEventos) {
    SpyEventBus bus;
    DiceService service(bus, DiceHandler(42));
    try { service.roll(DiceType::d6, 0); } catch (...) {}
    EXPECT_EQ(bus.capturedEvents.size(), 0u);
}
