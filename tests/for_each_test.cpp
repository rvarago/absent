#include <optional>

#include <catch2/catch.hpp>

#include <absent/for_each.h>

using namespace rvarago::absent;

SCENARIO("for_each provides a way to perform a side-effect in the value wrapped inside an optional<A>", "[for_each]") {

    GIVEN("An optional<int>") {

        int counter = 0;
        auto add_counter = [&counter](auto v) { counter += v; };

        WHEN("empty") {
            std::optional<int> none;

            THEN("do nothing") {
                for_each(none, add_counter);

                CHECK(counter == 0);
            }
        }

        WHEN("not empty") {
            std::optional<int> some{1};

            THEN("perform the side-effect of incrementing the counter") {
                for_each(some, add_counter);

                CHECK(counter == 1);
            }
        }
    }
}
