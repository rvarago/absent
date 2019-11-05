#include <absent/absent.h>
#include <absent/adapters/either.h>

#include <stdexcept>

#include <catch2/catch.hpp>

namespace {

    using namespace rvarago::absent;
    using adapters::either;

    struct Error{};
    struct Person{};
    struct Address{};

    template<typename Expected, typename A, typename E>
    void expect_alternative_of_type(either<A, E> e) {
        CHECK(std::holds_alternative<Expected>(e));
    }

    SCENARIO( "either lawfully works as a nullable", "[either]" ) {

        GIVEN( "An either" ) {

            WHEN( "empty" ) {

                THEN( "return a new empty nullable" ) {
                    auto const find_address = [](auto) { return either<Address, Error>{Address{}}; };
                    auto const get_zip_code = [](auto) { return 42; };
                    auto const either_error = either<Person, Error>{Error{}};

                    auto const either_zip_code = either_error >> find_address | get_zip_code;

                    expect_alternative_of_type<Error>(either_zip_code);
                }

                THEN( "return the result of calling the fallback function" ) {
                    auto const to_minus_one = [] { return -1; };

                    auto const either_error = either<int, Error>{Error{}};

                    CHECK(eval(either_error, to_minus_one) == -1);
                }
            }

            WHEN( "not empty" ) {

                THEN( "return a new transformed nullable" ) {
                    auto const find_address = [](auto) { return either<Address, Error>{Address{}}; };
                    auto const get_zip_code = [](auto) { return 42; };
                    auto const either_person = either<Person, Error>{Person{}};

                    auto const either_zip_code = either_person >> find_address | get_zip_code;

                    expect_alternative_of_type<int>(either_zip_code);
                    CHECK(std::get<int>(either_zip_code) == 42);
                }

                THEN( "apply foreach to increment the counter" ) {
                    int counter = 0;
                    auto const add_counter = [&counter](auto v) { counter += v; };

                    either<int, Error> const either_one = either<int, Error>{1};
                    foreach(either_one, add_counter);

                    expect_alternative_of_type<int>(either_one);
                    CHECK(counter == 1);
                }

            }

            WHEN( "wrapping a function that may throw an exception" ) {
                AND_WHEN("throw an excepted exception") {

                    auto always_throw = []() -> int {
                        throw std::runtime_error{"failed"};
                    };

                    THEN( "return a new failed either" ) {
                        auto const failed = attempt<either, std::exception>::run(always_throw);
                        expect_alternative_of_type<std::exception>(failed);
                    }
                }

                AND_WHEN("do not throw an exception") {

                    auto never_throw = []() -> int {
                        return 42;
                    };

                    THEN( "return the result inside a non-failed either" ) {
                        auto const success = attempt<either, std::exception>::run(never_throw);
                        expect_alternative_of_type<int>(success);
                        CHECK(std::get<int>(success) == 42);
                    }
                }
            }

        }
    }
}