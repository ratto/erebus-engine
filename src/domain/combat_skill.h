#pragma once
#include <string>
#include <vector>

namespace erebus::domain {

enum class CombatSkillType {
    Melee,   // ataque + defesa, custo 1:1
    Ranged,  // apenas ataque, custo 2:1
    Shield   // apenas defesa, custo 2:1
};

struct CombatSkillAllocation {
    CombatSkillType type;
    int attackBase;
    int defenseBase;
    int pointsAllocated;  // total de PP gastos
    int attackPoints;     // pp alocados em ataque
    int defensePoints;    // pp alocados em defesa
    bool isDefault;       // true para pericias gratuitas (ex: Briga)
};

struct CombatSkillResult {
    bool valid;
    int attackValue;
    int defenseValue;
    std::vector<std::string> errors;
};

} // namespace erebus::domain
