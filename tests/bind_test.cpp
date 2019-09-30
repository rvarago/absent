#include <absent/combinators/bind.h>

#include <optional>
#include <string>

#include <catch2/catch.hpp>

namespace {

    using namespace rvarago::absent;

    SCENARIO( "bind provides a generic and type-safe way to map and then flat a nullable", "[bind]" ) {

        GIVEN( "A nullable" ) {

            WHEN( "empty" ) {
                THEN( "return a new empty nullable" ) {
                    auto const increment_as_some = [](auto v) { return std::optional{v + 1}; };

                    std::optional<int> const none = std::nullopt;

                    CHECK((none >> increment_as_some) == std::nullopt);
                }
            }

            WHEN( "not empty" ) {
                THEN( "return a new nullable with the mapped and flattened value" ) {
                    auto const increment_as_some = [](auto v) { return std::optional{v + 1}; };

                    std::optional<int> const some_zero = std::optional{0};

                    CHECK((some_zero >> increment_as_some) == std::optional{1});
                }
            }

            WHEN( "not empty and mapped to a new type" ) {
                THEN( "return a new nullable with the mapped and flattened value of the new type" ) {
                    auto const string_to_int_as_some = [](auto v) { return std::optional{std::stoi(v)}; };
                    auto const int_to_string_as_some = [](auto v) { return std::optional{std::to_string(v)}; };

                    std::optional<std::string> const some_zero_as_string = std::optional{"0"};

                    CHECK((some_zero_as_string >> string_to_int_as_some) == std::optional{0});
                    CHECK((some_zero_as_string >> string_to_int_as_some >> int_to_string_as_some) == std::optional{"0"});
                }
            }

            WHEN( "the first is not empty but the second is empty" ) {
                THEN( "return an empty nullable" ) {
                    auto const string_to_int_as_some = [](auto v) { return std::optional{std::stoi(v)}; };
                    auto const int_to_string_as_none = [](auto) -> std::optional<std::string> { return std::nullopt; };

                    std::optional<std::string> const some_zero_as_string = std::optional{"0"};

                    CHECK((some_zero_as_string >> string_to_int_as_some >> int_to_string_as_none) == std::nullopt);
                }
            }

            WHEN( "the mapping function is a member function" ) {
                THEN( "do the same as the free function version" ) {
                    struct Person {
                        std::optional<int> id_as_none() const { return std::nullopt; }
                        std::optional<int> id_as_some() const { return std::optional{1}; }
                    };

                    std::optional<Person> const none_person = std::nullopt;
                    CHECK((none_person >> &Person::id_as_some) == std::nullopt);
                    CHECK((none_person >> &Person::id_as_none) == std::nullopt);

                    std::optional<Person> const some_person = std::optional{Person{}};
                    CHECK((some_person >> &Person::id_as_some) == std::optional{1});
                    CHECK((some_person >> &Person::id_as_none) == std::nullopt);
                }
            }
        }
    }
}