#include <optional>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <absent/attempt.h>

using namespace rvarago::absent;

SCENARIO("attempt provides a way to wrap a function that may throw an exception into an optional<A>", "[attempt]") {

    GIVEN("A function that may throw") {

        WHEN("throw an exception") {

            auto throw_runtime_error = []() -> int { throw std::runtime_error{"404"}; };

            AND_WHEN("exception is of expected type") {

                THEN("return a new empty optional<int>") {
                    std::optional<int> empty = attempt(throw_runtime_error);
                    CHECK(empty == std::nullopt);
                }
            }

            AND_WHEN("exception is not of expected type") {

                THEN("propagate the exception previously thrown") {
                    CHECK_THROWS_AS(attempt<std::logic_error>(throw_runtime_error), std::runtime_error);
                }
            }
        }

        WHEN("do not throw an exception") {

            auto never_throw = []() -> int { return 200; };

            THEN("return the result inside a non-empty optional<int>") {
                std::optional<int> success = attempt(never_throw);
                CHECK(success == std::optional{200});
            }
        }
    }
}