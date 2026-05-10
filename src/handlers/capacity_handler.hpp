#pragma once
#include "ports/i_capacity_calculator.hpp"
#include "domain/capacity_result.hpp"

namespace erebus::handlers {

class CapacityHandler : public erebus::ports::ICapacityCalculator {
public:
    erebus::domain::CapacityYResult    calculateY(double k, int attribute) override;
    erebus::domain::CapacityKResult    calculateK(double y) override;
    erebus::domain::DamageBonusResult  calculateDamageBonus(int fr) override;
};

} // namespace erebus::handlers
