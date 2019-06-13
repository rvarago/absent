#include <absent/absent.h>
#include <absent/adapters/either.h>

#include <gtest/gtest.h>

using namespace rvarago::absent;
using nullable::either;

namespace {

    struct error final {};
    struct person final {};
    struct address final {};

    template<typename Expected, typename A, typename E>
    void expect_alternative_of_type(either<A, E> const &it) {
        EXPECT_TRUE(std::holds_alternative<Expected>(it));
    }


    TEST(either, given_andEither_when_notError_shouldApplyForeachToIncrementCounter) {
        int counter = 0;
        auto const add_to_counter = [&counter](auto const &a) { counter += a; };

        auto one = either<int, error>{1};
        foreach(one, add_to_counter);

        expect_alternative_of_type<int>(one);
        EXPECT_EQ(1, counter);
    }

    TEST(either, given_anEither_when_IsError_should_ReturnError) {
        auto const find_person_error = [] { return either<person, error>{error{}}; };
        auto const find_address = [](auto const &) { return either<address, error>{address{}}; };
        auto const zip_code = [](auto const &) { return 42; };

        auto const either_zip_code = find_person_error() >> find_address | zip_code;

        expect_alternative_of_type<error>(either_zip_code);
    }

    TEST(either, given_anEither_when_NotError_shouldReturnNewTransformedEither) {
        auto const find_person = [] { return either<person, error>{person{}}; };
        auto const find_address = [](auto const &) { return either<address, error>{address{}}; };
        auto const zip_code = [](auto const &) { return 42; };

        auto const either_zip_code = find_person() >> find_address | zip_code;

        expect_alternative_of_type<int>(either_zip_code);
        EXPECT_EQ(42, std::get<int>(either_zip_code));
    }

}