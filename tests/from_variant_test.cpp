#include <absent/support/from_variant.h>

#include <string>

#include <catch2/catch_all.hpp>

using namespace rvarago::absent;

SCENARIO("from_variant provides a way to go from a variant<Rest..> to optional<A>", "[from_variant]") {

    GIVEN("A variant<int, string>") {

        std::variant<int, std::string> variant;

        WHEN("in the unexpected string alternative") {

            variant = std::string{"404"};

            THEN("return an empty optional<int>") {
                std::optional<int> none = from_variant<int>(variant);
                CHECK(none == std::nullopt);
            }
        }

        WHEN("in the expected int alternative") {

            variant = 42;

            THEN("return a non-empty optional<int> wrapping the obtained int") {
                std::optional<int> some = from_variant<int>(variant);
                CHECK(some == std::optional<int>{42});
            }
        }
    }
}
