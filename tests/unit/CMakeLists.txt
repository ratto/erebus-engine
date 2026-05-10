add_executable(unit_tests
    smoke_test.cpp
    dice_test.cpp
    combat_skill_handler_test.cpp
    capacity_handler_test.cpp
)

target_link_libraries(unit_tests
    GTest::gtest_main
    erebus_core
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
