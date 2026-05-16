#include "services/capacity_service.hpp"

namespace erebus::services {

CapacityService::CapacityService(erebus::ports::ICapacityCalculator& calculator)
    : calculator_(calculator) {}

erebus::domain::CapacityYResult CapacityService::calculateY(double k, int attribute) {
    return calculator_.calculateY(k, attribute);
}

erebus::domain::CapacityKResult CapacityService::calculateK(double y) {
    return calculator_.calculateK(y);
}

erebus::domain::DamageBonusResult CapacityService::calculateDamageBonus(int fr) {
    return calculator_.calculateDamageBonus(fr);
}

} // namespace erebus::services
