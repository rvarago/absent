#include <optional>
#include <string>

#include <catch2/catch.hpp>

#include <absent/fmap.h>

using namespace rvarago::absent;

SCENARIO("fmap provides a way to map {optional<A>, f: A -> B} to optional<B>", "[fmap]") {

    GIVEN("A function int -> string") {

        auto to_string = [](int x) -> std::string { return std::to_string(x); };

        AND_GIVEN("An optional<int>") {

            WHEN("empty") {
                std::optional<int> none;

                THEN("return a new empty optional<string>") {
                    std::optional<std::string> mapped_none = none | to_string;
                    CHECK(mapped_none == std::nullopt);
                }
            }

            WHEN("not empty") {
                std::optional<int> some{200};

                THEN("return a non-empty and mapped optional<string>") {
                    std::optional<std::string> mapped_some = some | to_string;
                    CHECK(mapped_some == std::optional{std::string{"200"}});
                }
            }
        }
    }

    GIVEN("A function Person -> string") {

        struct Person {
            std::string id() const {
                return std::string{"200"};
            }
        };

        AND_GIVEN("An optional<Person>") {

            WHEN("empty") {
                std::optional<Person> none;

                THEN("return a new empty optional<string>") {
                    std::optional<std::string> mapped_none = none | &Person::id;
                    CHECK(mapped_none == std::nullopt);
                }
            }

            WHEN("not empty") {
                std::optional<Person> some{Person{}};

                THEN("return a non-empty and mapped optional<string>") {
                    std::optional<std::string> mapped_some = some | &Person::id;
                    CHECK(mapped_some == std::optional{std::string{"200"}});
                }
            }
        }
    }
}
