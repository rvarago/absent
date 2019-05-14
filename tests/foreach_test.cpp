#include <absent/combinators/foreach.h>

#include <optional>
#include <string>

#include <gtest/gtest.h>

using namespace rvarago::absent;

TEST(foreach, given_AnOptional_when_Empty_should_DoNothing) {
    int counter = 0;
    auto const add_to_counter = [&counter](auto const& a){ counter += a; };

    std::optional<int> const empty_optional;

    foreach(empty_optional, add_to_counter);

    EXPECT_EQ(0, counter);
}

TEST(foreach, given_AnOptional_when_NotEmpty_should_RunTheEffectToAddToTheCounter) {
    int counter = 0;
    auto const add_to_counter = [&counter](auto const& a){ counter += a; };

    std::optional<int> const empty_optional{1};

    foreach(empty_optional, add_to_counter);

    EXPECT_EQ(1, counter);
}
