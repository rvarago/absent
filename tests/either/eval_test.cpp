#include <absent/adapters/either/eval.h>

#include <optional>

#include <catch2/catch_all.hpp>

using namespace rvarago::absent::adapters::either;
using rvarago::absent::adapters::types::either;

SCENARIO("eval provides a way to lazily go from either<A, E> to A", "[either-eval]") {

    struct Error {};

    GIVEN("An either<int, Error>") {

        auto to_minus_one = [] { return -1; };

        WHEN("invalid") {
            either<int, Error> invalid{Error{}};

            THEN("return the result of calling the fallback function") {
                int value = eval(invalid, to_minus_one);
                CHECK(value == -1);
            }
        }

        WHEN("valid") {
            either<int, Error> valid{1};

            THEN("return the wrapped value") {
                int value = eval(valid, to_minus_one);
                CHECK(value == 1);
            }
        }
    }
}