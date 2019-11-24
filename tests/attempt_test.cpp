#include <absent/absent.h>

#include <optional>
#include <stdexcept>

#include <catch2/catch.hpp>

namespace {

using namespace rvarago::absent;

SCENARIO("attempt provides a generic and type-safe way to wrap an exception into a nullable", "[attempt]") {

    GIVEN("A function that may throw") {

        WHEN("throw an exception") {

            auto throw_runtime_error = []() -> int { throw std::runtime_error{""}; };

            AND_WHEN("exception is of expected type") {
                THEN("return a new empty nullable") {
                    auto const empty = attempt<std::optional>::or_catch(throw_runtime_error);
                    CHECK(empty == std::nullopt);
                }
            }

            AND_WHEN("exception is not of expected type") {
                THEN("propagate the exception previously thrown") {
                    CHECK_THROWS_AS(attempt<std::optional>::or_catch<std::logic_error>(throw_runtime_error),
                                    std::runtime_error);
                }
            }
        }

        WHEN("do not throw an exception") {

            auto never_throw = []() -> int { return 42; };

            THEN("return the result inside a non-empty nullable") {
                auto const success = attempt<std::optional>::or_catch(never_throw);
                CHECK(success == std::optional{42});
            }
        }
    }
}
}