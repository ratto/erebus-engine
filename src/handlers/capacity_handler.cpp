#include "handlers/capacity_handler.hpp"
#include <cmath>

namespace erebus::handlers {

erebus::domain::CapacityYResult CapacityHandler::calculateY(double k, int attribute) {
    double y = k * std::pow(2.0, static_cast<double>(attribute) / 6.0);
    return { y };
}

erebus::domain::CapacityKResult CapacityHandler::calculateK(double y) {
    double k6  = y / std::pow(2.0, 6.0  / 6.0);
    double k10 = y / std::pow(2.0, 10.0 / 6.0);
    double k18 = y / std::pow(2.0, 18.0 / 6.0);
    double k = (k6 + k10 + k18) / 3.0;
    return { k };
}

erebus::domain::DamageBonusResult CapacityHandler::calculateDamageBonus(int fr) {
    // ceil((fr - 13.5) / 2.0), minimum 0.
    // Sistema Daemon corrected formula:
    //   FR 14    -> ceil(0.25)  = 1
    //   FR 15-16 -> ceil(0.75–1.25) = 1–2, etc.
    //   FR <= 13 -> 0 (minimum)
    int bonus = std::max(0, static_cast<int>(std::ceil((fr - 13.5) / 2.0)));
    return { bonus };
}

} // namespace erebus::handlers
