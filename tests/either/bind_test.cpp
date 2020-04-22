
#include <string>

#include <catch2/catch.hpp>

#include <absent/adapters/either/bind.h>

using namespace rvarago::absent::adapters::either;
using rvarago::absent::adapters::types::either;

SCENARIO("bind provides a way to map {either<A, E>, f: A -> either<B, E>} to either<B, E>", "[either-bind]") {

    struct Error {
        std::string code;
        explicit Error(std::string the_code) : code{std::move(the_code)} {
        }

        bool operator==(Error const &rhs) const {
            return code == rhs.code;
        }
    };

    GIVEN("A function int -> either<string, Error>") {

        auto to_string = [](int x) -> either<std::string, Error> {
            return either<std::string, Error>{std::to_string(x)};
        };

        AND_GIVEN("An either<int, Error>") {

            WHEN("invalid") {
                either<int, Error> invalid{Error{"404"}};

                THEN("return a new invalid either<string, Error>") {
                    either<std::string, Error> bound_invalid = invalid >> to_string;
                    CHECK(bound_invalid == either<std::string, Error>{Error{"404"}});
                }
            }

            WHEN("valid") {
                either<int, Error> valid{200};

                THEN("return a valid and bound either<string, Error>") {
                    either<std::string, Error> bound_some = valid >> to_string;
                    CHECK(bound_some == either<std::string, Error>{std::string{"200"}});
                }
            }
        }
    }

    GIVEN("A function Person -> either<string, Error>") {

        struct Person {
            either<std::string, Error> id() const {
                return either<std::string, Error>{std::string{"200"}};
            }
        };

        AND_GIVEN("An either<Person, Error>") {

            WHEN("invalid") {
                either<Person, Error> invalid{Error{"404"}};

                THEN("return a new invalid either<string, Error>") {
                    either<std::string, Error> bound_invalid = invalid >> &Person::id;
                    CHECK(bound_invalid == either<std::string, Error>{Error{"404"}});
                }
            }

            WHEN("valid") {
                either<Person, Error> valid{Person{}};

                THEN("return a valid and bound either<string, Error>>") {
                    either<std::string, Error> bound_valid = valid >> &Person::id;
                    CHECK(bound_valid == either<std::string, Error>{std::string{"200"}});
                }
            }
        }
    }
}
