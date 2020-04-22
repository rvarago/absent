#include <optional>

#include <catch2/catch.hpp>

#include <absent/adapters/either/for_each.h>

using namespace rvarago::absent::adapters::either;
using rvarago::absent::adapters::types::either;

SCENARIO("for_each provides a way to perform a side-effect in the value wrapped inside an either<A, E>",
         "[either-for_each]") {

    struct Error {};

    GIVEN("An either<int, Error>") {

        int counter = 0;
        auto add_counter = [&counter](auto v) { counter += v; };

        WHEN("invalid") {
            either<int, Error> invalid = Error{};

            THEN("do nothing") {
                for_each(invalid, add_counter);

                CHECK(counter == 0);
            }
        }

        WHEN("valid") {
            either<int, Error> valid = either<int, Error>{1};

            THEN("perform the side-effect of incrementing the counter") {
                for_each(valid, add_counter);

                CHECK(counter == 1);
            }
        }
    }
}
