#pragma once
#include "ports/i_combat_skill_validator.h"

namespace erebus::handlers {

class CombatSkillHandler : public erebus::ports::ICombatSkillValidator {
public:
    erebus::domain::CombatSkillResult validate(
        const erebus::domain::CombatSkillAllocation& allocation) const override;
};

} // namespace erebus::handlers
