#include <absent/absent.h>
#include <absent/adapters/unique_ptr.h>

#include <gtest/gtest.h>

using namespace rvarago::absent;

namespace {

    struct person final {};
    struct address final {};

    TEST(unique_ptr, given_ANullable_when_InvokeAbsent_should_MoveItAndLeaveItNullAfterwards) {
        auto zero = std::make_unique<int>(0);
        auto const one = std::move(zero) | [](auto value) { return value + 1; };

        EXPECT_FALSE(zero);
        EXPECT_TRUE(one);
        EXPECT_EQ(1, *one);
    }

    TEST(unique_ptr, given_ANullable_when_NotEmpty_should_ApplyForeachToIncrementCounter) {
        int counter = 0;
        auto const add_to_counter = [&counter](auto const &a) { counter += a; };

        auto one = std::make_unique<int>(1);
        foreach(std::move(one), add_to_counter);

        EXPECT_FALSE(one);
        EXPECT_EQ(1, counter);
    }

    TEST(unique_ptr, given_ANullable_when_Empty_should_ReturnAnEmptyNullable) {
        auto const find_person_empty = []() -> std::unique_ptr<person> { return nullptr; };
        auto const find_address = [](auto const &) { return std::make_unique<address>(); };
        auto const zip_code = [](auto const &) { return 42; };

        EXPECT_FALSE(find_person_empty() >> find_address | zip_code);
    }

    TEST(unique_ptr, given_ANullable_when_NotEmpty_should_ReturnANewTransformedNullable) {
        auto const find_person = [] { return std::make_unique<person>(); };
        auto const find_address = [](auto const &) { return std::make_unique<address>(); };
        auto const zip_code = [](auto const &) { return 42; };

        EXPECT_EQ(42, *(find_person() >> find_address | zip_code));
    }

    TEST(unique_ptr, given_ANullable_when_Empty_should_CallTheFallback) {
        auto const to_minus_one = [] { return -1; };

        EXPECT_EQ(-1, eval(std::unique_ptr<int>{}, to_minus_one));
    }

    TEST(unique_ptr, given_ANullable_when_NotEmpty_should_CallCtorAndDtorTheSameNumberOfTimes) {
        static int calls_to_ctor = 0;
        static int calls_to_dtor = 0;

        struct foo {
            foo() {++calls_to_ctor;}
            ~foo() {++calls_to_dtor;}
        };

        auto const new_some = [] { return std::make_unique<foo>(); };
        auto const to_new_some = [](auto const &) { return std::make_unique<foo>(); };
        auto const to_new_some_but_throw_before = [](auto const &) { throw 0; return std::make_unique<foo>(); };
        auto const to_number = [](auto const &) { return 42; };

        try {new_some() >> to_new_some >> to_new_some_but_throw_before >> to_new_some | to_number;} catch (...) {}
        EXPECT_EQ(calls_to_ctor, calls_to_dtor) << "Unequal number of calls to Ctor and Dtor: it might cause memory leak";
    }

}