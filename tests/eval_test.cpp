#include <absent/combinators/eval.h>

#include <optional>
#include <string>

#include <gtest/gtest.h>

using namespace rvarago::absent;

namespace {

    TEST(eval, given_anOptional_when_Empty_should_CallTheFallback) {
        auto const to_minus_one = [] { return -1; };

        std::optional<int> const none;

        EXPECT_EQ(-1, eval(none, to_minus_one));
    }

    TEST(eval, given_anOptional_when_NotEmpty_should_ReturnTheWrappedValue) {
        auto const to_minus_one = [] { return -1; };

        auto const one = std::optional<int>{1};

        EXPECT_EQ(1, eval(one, to_minus_one));
    }

}