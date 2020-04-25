#include <functional>
#include <optional>
#include <string>

#include <catch2/catch.hpp>

#include <absent/and_then.h>

using namespace rvarago::absent;

SCENARIO("and_then provides a way to and_then {optional<A>, f: A -> optional<B>} to optional<B>", "[and_then]") {

    GIVEN("A function int -> optional<string>") {

        auto to_string = [](int x) -> std::optional<std::string> { return std::optional{std::to_string(x)}; };

        AND_GIVEN("An optional<int>") {

            WHEN("empty") {
                std::optional<int> none;

                THEN("return a new empty optional<string>") {
                    std::optional<std::string> bound_none = none >> to_string;
                    CHECK(bound_none == std::nullopt);
                }
            }

            WHEN("not empty") {
                std::optional<int> some{200};

                THEN("return a non-empty and bound optional<string>") {
                    std::optional<std::string> bound_some = some >> to_string;
                    CHECK(bound_some == std::optional{std::string{"200"}});
                }
            }
        }
    }

    GIVEN("A function Person -> optional<string>") {

        struct Person {
            std::optional<std::string> id() const {
                return std::optional<std::string>{"200"};
            }
        };

        AND_GIVEN("An optional<Person>") {

            WHEN("empty") {
                std::optional<Person> none;

                THEN("return a new empty optional<string>") {
                    std::optional<std::string> bound_none = none >> std::mem_fn(&Person::id);
                    CHECK(bound_none == std::nullopt);
                }
            }

            WHEN("not empty") {
                std::optional<Person> some{Person{}};

                THEN("return a non-empty and bound optional<string>") {
                    std::optional<std::string> bound_some = some >> std::mem_fn(&Person::id);
                    CHECK(bound_some == std::optional{std::string{"200"}});
                }
            }
        }
    }
}
