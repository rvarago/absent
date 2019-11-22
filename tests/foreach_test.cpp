#include <absent/combinators/foreach.h>

#include <optional>
#include <string>

#include <catch2/catch.hpp>

namespace {

using namespace rvarago::absent;

SCENARIO("foreach provides a generic and type-safe way to perform a side-effect in the value wrapped inside a nullable",
         "[foreach]") {

    GIVEN("A nullable") {

        int counter = 0;
        auto const add_counter = [&counter](auto v) { counter += v; };

        WHEN("empty") {
            THEN("do nothing") {
                std::optional<int> const none = std::nullopt;

                foreach (none, add_counter)
                    ;

                CHECK(counter == 0);
            }
        }

        WHEN("not empty") {
            THEN("perform the side-effect of incrementing the counter") {
                std::optional<int> const some_one{1};

                foreach (some_one, add_counter)
                    ;

                CHECK(counter == 1);
            }
        }
    }
}
}
