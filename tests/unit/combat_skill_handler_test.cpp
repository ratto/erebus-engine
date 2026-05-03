#include <gtest/gtest.h>
#include "handlers/combat_skill_handler.h"
#include "domain/combat_skill.h"

using namespace erebus::domain;
using namespace erebus::handlers;

class CombatSkillHandlerTest : public ::testing::Test {
protected:
    CombatSkillHandler handler;
};

// --- Melee ---

TEST_F(CombatSkillHandlerTest, MeleeValidAllocation) {
    CombatSkillAllocation alloc{
        CombatSkillType::Melee,
        /*attackBase=*/14, /*defenseBase=*/14,
        /*pointsAllocated=*/22,
        /*attackPoints=*/12, /*defensePoints=*/10,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.attackValue,  26);
    EXPECT_EQ(result.defenseValue, 24);
    EXPECT_TRUE(result.errors.empty());
}

TEST_F(CombatSkillHandlerTest, MeleeMaxPoints) {
    CombatSkillAllocation alloc{
        CombatSkillType::Melee,
        /*attackBase=*/10, /*defenseBase=*/10,
        /*pointsAllocated=*/50,
        /*attackPoints=*/25, /*defensePoints=*/25,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.attackValue,  35);
    EXPECT_EQ(result.defenseValue, 35);
}

TEST_F(CombatSkillHandlerTest, MeleeBelowMinPoints) {
    CombatSkillAllocation alloc{
        CombatSkillType::Melee,
        /*attackBase=*/10, /*defenseBase=*/10,
        /*pointsAllocated=*/8,
        /*attackPoints=*/4, /*defensePoints=*/4,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_FALSE(result.valid);
    ASSERT_EQ(result.errors.size(), 1u);
    EXPECT_EQ(result.errors[0], "COMBAT_SKILL_POINTS_MIN");
}

TEST_F(CombatSkillHandlerTest, MeleeAboveMaxPoints) {
    CombatSkillAllocation alloc{
        CombatSkillType::Melee,
        /*attackBase=*/10, /*defenseBase=*/10,
        /*pointsAllocated=*/52,
        /*attackPoints=*/26, /*defensePoints=*/26,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_FALSE(result.valid);
    ASSERT_EQ(result.errors.size(), 1u);
    EXPECT_EQ(result.errors[0], "COMBAT_SKILL_POINTS_MAX");
}

TEST_F(CombatSkillHandlerTest, MeleeMismatchedPoints) {
    CombatSkillAllocation alloc{
        CombatSkillType::Melee,
        /*attackBase=*/10, /*defenseBase=*/10,
        /*pointsAllocated=*/22,
        /*attackPoints=*/10, /*defensePoints=*/10, // soma 20, nao 22
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_FALSE(result.valid);
    ASSERT_EQ(result.errors.size(), 1u);
    EXPECT_EQ(result.errors[0], "COMBAT_SKILL_MELEE_POINT_MISMATCH");
}

// --- Ranged ---

TEST_F(CombatSkillHandlerTest, RangedValidAllocation) {
    CombatSkillAllocation alloc{
        CombatSkillType::Ranged,
        /*attackBase=*/14, /*defenseBase=*/0,
        /*pointsAllocated=*/18,
        /*attackPoints=*/9, /*defensePoints=*/0,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.attackValue,  23);
    EXPECT_EQ(result.defenseValue, 0);
    EXPECT_TRUE(result.errors.empty());
}

TEST_F(CombatSkillHandlerTest, RangedWithDefensePoints) {
    CombatSkillAllocation alloc{
        CombatSkillType::Ranged,
        /*attackBase=*/14, /*defenseBase=*/0,
        /*pointsAllocated=*/18,
        /*attackPoints=*/7, /*defensePoints=*/2,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_FALSE(result.valid);
    bool hasError = false;
    for (const auto& e : result.errors) {
        if (e == "COMBAT_SKILL_RANGED_NO_DEFENSE") hasError = true;
    }
    EXPECT_TRUE(hasError);
}

// --- Shield ---

TEST_F(CombatSkillHandlerTest, ShieldValidAllocation) {
    CombatSkillAllocation alloc{
        CombatSkillType::Shield,
        /*attackBase=*/0, /*defenseBase=*/14,
        /*pointsAllocated=*/20,
        /*attackPoints=*/0, /*defensePoints=*/10,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.attackValue,  0);
    EXPECT_EQ(result.defenseValue, 24);
    EXPECT_TRUE(result.errors.empty());
}

TEST_F(CombatSkillHandlerTest, ShieldWithAttackPoints) {
    CombatSkillAllocation alloc{
        CombatSkillType::Shield,
        /*attackBase=*/0, /*defenseBase=*/14,
        /*pointsAllocated=*/20,
        /*attackPoints=*/3, /*defensePoints=*/7,
        /*isDefault=*/false
    };
    auto result = handler.validate(alloc);
    EXPECT_FALSE(result.valid);
    bool hasError = false;
    for (const auto& e : result.errors) {
        if (e == "COMBAT_SKILL_SHIELD_NO_ATTACK") hasError = true;
    }
    EXPECT_TRUE(hasError);
}

// --- Caso especial: Briga gratuita ---

TEST_F(CombatSkillHandlerTest, BrigaDefaultCharacter) {
    // Personagem com DEX 14, 0 PP alocados — Briga e gratuita
    CombatSkillAllocation alloc{
        CombatSkillType::Melee,
        /*attackBase=*/14, /*defenseBase=*/14,
        /*pointsAllocated=*/0,
        /*attackPoints=*/0, /*defensePoints=*/0,
        /*isDefault=*/true
    };
    auto result = handler.validate(alloc);
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.attackValue,  14);
    EXPECT_EQ(result.defenseValue, 14);
    EXPECT_TRUE(result.errors.empty());
}
