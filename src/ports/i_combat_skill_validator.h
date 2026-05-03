#pragma once
#include "domain/combat_skill.h"

namespace erebus::ports {

class ICombatSkillValidator {
public:
    virtual ~ICombatSkillValidator() = default;
    virtual erebus::domain::CombatSkillResult validate(
        const erebus::domain::CombatSkillAllocation& allocation) const = 0;
};

} // namespace erebus::ports
