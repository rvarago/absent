#include <absent/absent.h>
#include <absent/adapters/either.h>

#include <gtest/gtest.h>

using namespace rvarago::absent;
using adapters::either;

namespace {

    struct error final {};
    struct person final {};
    struct address final {};

    template<typename Expected, typename A, typename E>
    void expect_alternative_of_type(either<A, E> const &it) {
        EXPECT_TRUE(std::holds_alternative<Expected>(it));
    }


    TEST(either, given_ANullable_when_NotEmpty_should_ApplyForeachToIncrementTheCounter) {
        int counter = 0;
        auto const add_to_counter = [&counter](auto const &a) { counter += a; };

        auto one = either<int, error>{1};
        foreach(one, add_to_counter);

        expect_alternative_of_type<int>(one);
        EXPECT_EQ(1, counter);
    }

    TEST(either, given_ANullable_when_Empty_should_ReturnAnEmptyNullable) {
        auto const person_empty = either<person, error>{error{}};
        auto const find_address = [](auto const &) { return either<address, error>{address{}}; };
        auto const zip_code = [](auto const &) { return 42; };

        auto const either_zip_code = person_empty >> find_address | zip_code;

        expect_alternative_of_type<error>(either_zip_code);
    }

    TEST(either, given_ANullable_when_NotEmpty_should_ReturnANewTransformedNullable) {
        auto const person_some = either<person, error>{person{}};
        auto const find_address = [](auto const &) { return either<address, error>{address{}}; };
        auto const zip_code = [](auto const &) { return 42; };

        auto const either_zip_code = person_some >> find_address | zip_code;

        expect_alternative_of_type<int>(either_zip_code);
        EXPECT_EQ(42, std::get<int>(either_zip_code));
    }

    TEST(either, given_ANullable_when_Empty_should_CallTheFallback) {
        auto const to_minus_one = [] { return -1; };

        auto const none = either<int, error>{error{}};

        EXPECT_EQ(-1, eval(none, to_minus_one));
    }

}