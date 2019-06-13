#include <absent/combinators/fmap.h>

#include <optional>
#include <string>

#include <gtest/gtest.h>

using namespace rvarago::absent;

namespace {

    TEST(fmap, given_ANullable_when_Empty_should_ReturnAnEmptyNullable) {
        auto const increment = [](auto const &a) { return a + 1; };

        std::optional<int> const none;

        EXPECT_FALSE(none | increment);
        EXPECT_FALSE(none | increment | increment);
    }

    TEST(fmap, given_ANullable_when_NotEmpty_should_ReturnANewNullablelWithTheMappedValue) {
        auto const increment = [](auto const &a) { return a + 1; };

        std::optional<int> const some_zero{0};

        EXPECT_EQ(std::optional{1}, some_zero | increment);
        EXPECT_EQ(std::optional{2}, some_zero | increment | increment);
        EXPECT_EQ(std::optional{3}, some_zero | increment | increment | increment);
    }

    TEST(fmap, given_ANullable_when_NotEmptyAndMappedToANewType_should_ReturnANewNullableWithTheMappedValueOfTheNewType) {
        auto const string_to_int = [](auto const &a) { return std::stoi(a); };
        auto const int_to_string = [](auto const &a) { return std::to_string(a); };

        std::optional<std::string> const some_zero_as_string{"0"};

        EXPECT_EQ(std::optional{0}, some_zero_as_string | string_to_int);
        EXPECT_EQ(std::optional{"0"}, some_zero_as_string | string_to_int | int_to_string);
    }

    TEST(fmap, given_ANullableAndAMemberFunction_when_Mapping_should_ReturnTheMappedValueWrappedInANullable) {
        struct person {
            int id() const { return 1; }
        };

        EXPECT_EQ(std::optional{1}, std::optional{person{}} | &person::id);
        EXPECT_FALSE(std::optional<person>{} | &person::id);
    }

}