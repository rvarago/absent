#include <absent/combinators/eval.h>

#include <optional>

#include <catch2/catch.hpp>

namespace {

    using namespace rvarago::absent;

    SCENARIO( "eval provides a generic, type-safe, and lazy way to access a nullable", "[eval]" ) {

        GIVEN("A nullable") {

            auto const to_minus_one = [] { return -1; };

            WHEN("empty") {
                THEN("return the result of calling the fallback function") {
                    std::optional<int> const none = std::nullopt;

                    CHECK(eval(none, to_minus_one) == -1);
                }
            }

            WHEN("not empty") {
                THEN("return the wrapped value") {
                    auto const one = std::optional<int>{1};

                    CHECK(eval(one, to_minus_one) == 1);
                }
            }
        }
    }
}