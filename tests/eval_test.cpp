#include <absent/eval.h>

#include <optional>

#include <catch2/catch_all.hpp>

using namespace rvarago::absent;

SCENARIO("eval provides a way to lazily go from optional<A> to A", "[eval]") {

    GIVEN("An optional<int>") {

        auto to_minus_one = [] { return -1; };

        WHEN("empty") {
            std::optional<int> none;

            THEN("return the result of calling the fallback function") {
                int value = eval(none, to_minus_one);
                CHECK(value == -1);
            }
        }

        WHEN("not empty") {
            std::optional<int> one{1};

            THEN("return the wrapped value") {
                int value = eval(one, to_minus_one);
                CHECK(value == 1);
            }
        }
    }
}