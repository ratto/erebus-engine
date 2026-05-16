#pragma once
#include "domain/capacity_result.hpp"

namespace erebus::ports {

class ICapacityCalculator {
public:
    virtual ~ICapacityCalculator() = default;

    virtual erebus::domain::CapacityYResult    calculateY(double k, int attribute) = 0;
    virtual erebus::domain::CapacityKResult    calculateK(double y) = 0;
    virtual erebus::domain::DamageBonusResult  calculateDamageBonus(int fr) = 0;
};

} // namespace erebus::ports
