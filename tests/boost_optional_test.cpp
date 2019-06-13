#ifdef WITH_ADAPTER_FOR_BOOST_OPTIONAL

#include <absent/absent.h>
#include <absent/adapters/boost_optional.h>
#include <gtest/gtest.h>

using namespace rvarago::absent;

namespace {

    struct person final {};
    struct address final {};

    TEST(boost_optional, given_ANullable_when_Empty_should_ReturnAnEmptyNullable) {
        boost::optional<person> const person_empty = boost::none;
        auto const find_address = [](auto const &) { return boost::optional<address>{address{}}; };
        auto const zip_code = [](auto const &) { return 42; };

        EXPECT_FALSE(person_empty >> find_address | zip_code);
    }

    TEST(boost_optional, given_ANullable_when_NotEmpty_should_ReturnANewTransformedNullable) {
        auto const person_some =  boost::optional<person>{person{}};
        auto const find_address = [](auto const &) { return boost::optional<address>{address{}}; };
        auto const zip_code = [](auto const &) { return 42; };

        auto const maybe_zip_code = person_some >> find_address | zip_code;

        EXPECT_TRUE(maybe_zip_code);
        EXPECT_EQ(42, maybe_zip_code.value());
    }

    TEST(boost_optional, given_ANullable_when_Empty_should_CallTheFallback) {
        auto const to_minus_one = [] { return -1; };

        boost::optional<int> const none;

        EXPECT_EQ(-1, eval(none, to_minus_one));
    }

}

#endif