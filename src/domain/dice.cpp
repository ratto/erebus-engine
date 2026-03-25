#include "domain/dice.h"
#include <numeric>

namespace erebus::domain {

int DiceRoll::total() const {
    return std::accumulate(results.begin(), results.end(), 0);
}

} // namespace erebus::domain
