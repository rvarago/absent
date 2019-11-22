#include <absent/combinators/fmap.h>

#include <optional>
#include <string>

#include <catch2/catch.hpp>

namespace {

using namespace rvarago::absent;

SCENARIO("fmap provides a generic and type-safe way to map a nullable", "[fmap]") {

    GIVEN("A nullable") {

        WHEN("empty") {
            THEN("return a new empty nullable") {
                auto const increment = [](auto v) { return v + 1; };

                std::optional<int> const none = std::nullopt;

                CHECK((none | increment) == std::nullopt);
            }
        }

        WHEN("not empty") {
            THEN("return a new nullable with the mapped value") {
                auto const increment = [](auto v) { return v + 1; };

                std::optional<int> const some_zero{0};

                CHECK((some_zero | increment) == std::optional{1});
            }
        }

        WHEN("not empty and mapped to a new type") {
            THEN("return a new nullable with the mapped value of the new type") {
                auto const string_to_int = [](auto v) { return std::stoi(v); };
                auto const int_to_string = [](auto v) { return std::to_string(v); };

                std::optional<std::string> const some_zero_as_string{"0"};

                CHECK((some_zero_as_string | string_to_int) == std::optional{0});
                CHECK((some_zero_as_string | string_to_int | int_to_string) == std::optional{"0"});
            }
        }

        WHEN("the mapping function is a member function") {
            THEN("do the same as the free function version") {
                struct Person {
                    int id() const {
                        return 1;
                    }
                };

                std::optional<Person> const none_person = std::nullopt;
                CHECK((none_person | &Person::id) == std::nullopt);

                std::optional<Person> const some_person = std::optional{Person{}};
                CHECK((some_person | &Person::id) == std::optional{1});
            }
        }
    }
}
}