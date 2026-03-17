add_executable(integration_tests smoke_test.cpp)
target_link_libraries(integration_tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(integration_tests)
