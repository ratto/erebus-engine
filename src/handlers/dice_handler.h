#pragma once
#include "domain/dice.h"
#include <random>
#include <cstdint>

namespace erebus::handlers {

class DiceHandler {
public:
    // Semente aleatória por padrão; semente fixa permite testes determinísticos.
    explicit DiceHandler(std::uint32_t seed = std::random_device{}());

    // Rola o dado uma única vez e retorna o resultado.
    int rollOnce(erebus::domain::DiceType type);

private:
    std::mt19937 rng_;

    static int minValue(erebus::domain::DiceType type);
    static int maxValue(erebus::domain::DiceType type);
};

} // namespace erebus::handlers
