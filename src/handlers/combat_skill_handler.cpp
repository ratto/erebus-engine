#include "combat_skill_handler.h"

namespace erebus::handlers {

erebus::domain::CombatSkillResult CombatSkillHandler::validate(
    const erebus::domain::CombatSkillAllocation& allocation) const
{
    using namespace erebus::domain;

    CombatSkillResult result;
    result.valid        = true;
    result.attackValue  = 0;
    result.defenseValue = 0;

    // Pericias gratuitas (isDefault=true) ignoram regras de minimo/maximo de PP.
    if (!allocation.isDefault) {
        if (allocation.pointsAllocated < 10) {
            result.valid = false;
            result.errors.push_back("COMBAT_SKILL_POINTS_MIN");
            return result;
        }
        if (allocation.pointsAllocated > 50) {
            result.valid = false;
            result.errors.push_back("COMBAT_SKILL_POINTS_MAX");
            return result;
        }
    }

    switch (allocation.type) {
        case CombatSkillType::Melee: {
            // Custo 1:1 — soma de ataque + defesa deve ser igual ao total alocado
            if (allocation.attackPoints + allocation.defensePoints != allocation.pointsAllocated) {
                result.valid = false;
                result.errors.push_back("COMBAT_SKILL_MELEE_POINT_MISMATCH");
                return result;
            }
            result.attackValue  = allocation.attackBase  + allocation.attackPoints;
            result.defenseValue = allocation.defenseBase + allocation.defensePoints;
            break;
        }
        case CombatSkillType::Ranged: {
            // Custo 2:1 — apenas ataque pode ser comprado; sem defesa
            if (allocation.defensePoints != 0) {
                result.valid = false;
                result.errors.push_back("COMBAT_SKILL_RANGED_NO_DEFENSE");
                return result;
            }
            int attackPoints    = allocation.pointsAllocated / 2;
            result.attackValue  = allocation.attackBase + attackPoints;
            result.defenseValue = 0;
            break;
        }
        case CombatSkillType::Shield: {
            // Custo 2:1 — apenas defesa pode ser comprada; sem ataque
            if (allocation.attackPoints != 0) {
                result.valid = false;
                result.errors.push_back("COMBAT_SKILL_SHIELD_NO_ATTACK");
                return result;
            }
            int defensePoints   = allocation.pointsAllocated / 2;
            result.attackValue  = 0;
            result.defenseValue = allocation.defenseBase + defensePoints;
            break;
        }
    }

    return result;
}

} // namespace erebus::handlers
