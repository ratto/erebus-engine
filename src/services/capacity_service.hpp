#pragma once
#include "ports/i_capacity_calculator.hpp"
#include "domain/capacity_result.hpp"

namespace erebus::services {

class CapacityService {
public:
    explicit CapacityService(erebus::ports::ICapacityCalculator& calculator);

    erebus::domain::CapacityYResult    calculateY(double k, int attribute);
    erebus::domain::CapacityKResult    calculateK(double y);
    erebus::domain::DamageBonusResult  calculateDamageBonus(int fr);

private:
    erebus::ports::ICapacityCalculator& calculator_;
};

} // namespace erebus::services
