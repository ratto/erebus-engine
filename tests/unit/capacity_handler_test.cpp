#include <gtest/gtest.h>
#include "handlers/capacity_handler.hpp"

namespace {

class CapacityHandlerTest : public ::testing::Test {
protected:
    erebus::handlers::CapacityHandler handler;
};

// ---------------------------------------------------------------------------
// calculateY
// ---------------------------------------------------------------------------

// Y = K * 2^(attr/6)
// K=12.4, attr=10: 12.4 * 2^(10/6) = 12.4 * 3.1748 ≈ 39.37
TEST_F(CapacityHandlerTest, CalculateY_FR10_K12_4_ReturnsApprox39_37) {
    auto result = handler.calculateY(12.4, 10);
    EXPECT_NEAR(result.y, 39.37, 0.01);
}

// K=12.4, attr=16: 12.4 * 2^(16/6) = 12.4 * 6.3496 ≈ 78.74
TEST_F(CapacityHandlerTest, CalculateY_FR16_K12_4_ReturnsApprox78_74) {
    auto result = handler.calculateY(12.4, 16);
    EXPECT_NEAR(result.y, 78.74, 0.01);
}

TEST_F(CapacityHandlerTest, CalculateY_AttributeZero_ReturnsK) {
    // 2^(0/6) = 1, so Y = K * 1 = K
    auto result = handler.calculateY(12.4, 0);
    EXPECT_NEAR(result.y, 12.4, 0.01);
}

TEST_F(CapacityHandlerTest, CalculateY_Attribute6_DoubleK) {
    // 2^(6/6) = 2, so Y = K * 2
    auto result = handler.calculateY(10.0, 6);
    EXPECT_NEAR(result.y, 20.0, 0.01);
}

// ---------------------------------------------------------------------------
// calculateK
// ---------------------------------------------------------------------------

TEST_F(CapacityHandlerTest, CalculateK_Y39_37_ReturnsApprox12_3) {
    // Y=39.37 is produced by K=12.4 at attr=10; inverting gives K ≈ 12.34
    auto result = handler.calculateK(39.37);
    EXPECT_NEAR(result.k, 12.34, 0.5);
}

TEST_F(CapacityHandlerTest, CalculateK_PositiveY_ReturnsPositiveK) {
    auto result = handler.calculateK(100.0);
    EXPECT_GT(result.k, 0.0);
}

// ---------------------------------------------------------------------------
// calculateDamageBonus
// ---------------------------------------------------------------------------

TEST_F(CapacityHandlerTest, DamageBonus_FR14_Returns1) {
    // ceil((14 - 13.5) / 2.0) = ceil(0.25) = 1
    auto result = handler.calculateDamageBonus(14);
    EXPECT_EQ(result.damageBonus, 1);
}

TEST_F(CapacityHandlerTest, DamageBonus_FR15_Returns1) {
    // ceil((15 - 13.5) / 2.0) = ceil(0.75) = 1
    auto result = handler.calculateDamageBonus(15);
    EXPECT_EQ(result.damageBonus, 1);
}

TEST_F(CapacityHandlerTest, DamageBonus_FR16_Returns2) {
    // ceil((16 - 13.5) / 2.0) = ceil(1.25) = 2
    auto result = handler.calculateDamageBonus(16);
    EXPECT_EQ(result.damageBonus, 2);
}

TEST_F(CapacityHandlerTest, DamageBonus_FR17_Returns2) {
    // ceil((17 - 13.5) / 2.0) = ceil(1.75) = 2
    auto result = handler.calculateDamageBonus(17);
    EXPECT_EQ(result.damageBonus, 2);
}

TEST_F(CapacityHandlerTest, DamageBonus_FR18_Returns3) {
    // ceil((18 - 13.5) / 2.0) = ceil(2.25) = 3
    auto result = handler.calculateDamageBonus(18);
    EXPECT_EQ(result.damageBonus, 3);
}

TEST_F(CapacityHandlerTest, DamageBonus_FR20_Returns4) {
    // ceil((20 - 13.5) / 2.0) = ceil(3.25) = 4
    auto result = handler.calculateDamageBonus(20);
    EXPECT_EQ(result.damageBonus, 4);
}

TEST_F(CapacityHandlerTest, DamageBonus_FR1_Returns0) {
    // max(0, ceil((1 - 13.5) / 2.0)) = max(0, ceil(-6.25)) = max(0, -6) = 0
    auto result = handler.calculateDamageBonus(1);
    EXPECT_EQ(result.damageBonus, 0);
}

TEST_F(CapacityHandlerTest, DamageBonus_FR13_Returns0) {
    // max(0, ceil((13 - 13.5) / 2.0)) = max(0, ceil(-0.25)) = max(0, 0) = 0
    auto result = handler.calculateDamageBonus(13);
    EXPECT_EQ(result.damageBonus, 0);
}

} // anonymous namespace
