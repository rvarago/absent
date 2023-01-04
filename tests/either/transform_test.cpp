#include <absent/adapters/either/transform.h>

#include <string>
#include <utility>

#include <catch2/catch_all.hpp>

using namespace rvarago::absent::adapters::either;
using rvarago::absent::adapters::types::either;

SCENARIO("transform provides a way to map {either<A, E>, f: A -> B} to either<B, E>", "[either-transform]") {

    struct Error {
        std::string code;
        explicit Error(std::string the_code) : code{std::move(the_code)} {
        }

        bool operator==(Error const &rhs) const {
            return code == rhs.code;
        }
    };

    GIVEN("A function int -> string") {

        auto to_string = [](int x) -> std::string { return std::to_string(x); };

        AND_GIVEN("An either<int, Error>") {

            WHEN("invalid") {
                either<int, Error> invalid{Error{"404"}};

                THEN("return a new invalid either<string, Error>") {
                    either<std::string, Error> mapped_invalid = invalid | to_string;
                    CHECK(mapped_invalid == either<std::string, Error>{Error{"404"}});
                }
            }

            WHEN("valid") {
                either<int, Error> valid{200};

                THEN("return a valid and mapped either<string, Error>") {
                    either<std::string, Error> mapped_valid = valid | to_string;
                    CHECK(mapped_valid == either<std::string, Error>{std::string{"200"}});
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

        AND_GIVEN("An either<Person, Error>") {

            WHEN("invalid") {
                either<Person, Error> invalid{Error{"404"}};

                THEN("return a new invalid either<string, Error>") {
                    either<std::string, Error> mapped_invalid = invalid | &Person::id;
                    CHECK(mapped_invalid == either<std::string, Error>{Error{"404"}});
                }
            }

            WHEN("valid") {
                either<Person, Error> valid{Person{}};

                THEN("return a valid and mapped either<string, Error>") {
                    either<std::string, Error> mapped_valid = valid | &Person::id;
                    CHECK(mapped_valid == either<std::string, Error>{std::string{"200"}});
                }
            }
        }
    }
}
