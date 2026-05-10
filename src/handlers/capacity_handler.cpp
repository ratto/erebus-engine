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
    // Floor division: (fr - 14) / 2, minimum 0.
    // Matches the Sistema Daemon reference table:
    //   FR 14-15 -> 0, FR 16-17 -> 1, FR 18-19 -> 2, FR 20-21 -> 3, etc.
    int bonus = (fr >= 14) ? (fr - 14) / 2 : 0;
    return { bonus };
}

} // namespace erebus::handlers
