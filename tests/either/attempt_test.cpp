#include <absent/adapters/either/attempt.h>

#include <stdexcept>

#include <catch2/catch.hpp>

using namespace rvarago::absent::adapters::either;
using rvarago::absent::adapters::types::either;

SCENARIO("attempt provides a way to wrap a function that may throw an exception into an either<A, E>",
         "[either-attempt]") {

    GIVEN("A function that may throw") {

        WHEN("throw an exception") {

            auto throw_runtime_error = []() -> int { throw std::runtime_error{"404"}; };

            AND_WHEN("exception is of expected type") {

                THEN("return a new invalid either<int, BaseException>") {
                    either<int, std::exception> invalid = attempt(throw_runtime_error);
                    CHECK(std::holds_alternative<std::exception>(invalid));
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

            THEN("return the result inside a valid either<int, BaseExecption>") {
                either<int, std::exception> valid = attempt(never_throw);
                CHECK(std::holds_alternative<int>(valid));
                CHECK(std::get<int>(valid) == 200);
            }
        }
    }
}