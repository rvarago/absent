#include <absent/fmap.h>

#include <memory>
#include <optional>
#include <string>

#include <gtest/gtest.h>

using namespace rvarago::absent;

TEST(fmap, given_AnOptional_when_Empty_should_ReturnAnEmptyOptional) {
    auto const increment = [](auto const& a){ return a + 1; };

    std::optional<int> const empty_optional;

    EXPECT_FALSE(empty_optional & increment);
    EXPECT_FALSE(empty_optional & increment & increment);
}

TEST(fmap, given_AnOptional_when_NotEmpty_should_ReturnNewOptionalWithTheMappedValue) {
    auto const increment = [](auto const& a){ return a + 1; };

    std::optional<int> const some_zero{0};

    EXPECT_EQ(std::optional(1), some_zero & increment);
    EXPECT_EQ(std::optional(2), some_zero & increment & increment);
    EXPECT_EQ(std::optional(3), some_zero & increment & increment & increment);
}

TEST(fmap, given_AnOptional_when_NotEmptyAndMappedToANewType_should_ReturnNewOptionalWithTheMappedValueOfTheNewType) {
    auto const string_to_int = [](auto const& a){ return std::stoi(a); };
    auto const int_to_string = [](auto const& a){ return std::to_string(a); };

    std::optional<std::string> const some_zero_as_string{"0"};

    EXPECT_EQ(std::optional(0), some_zero_as_string & string_to_int);
    EXPECT_EQ(std::optional("0"), some_zero_as_string & string_to_int & int_to_string);
}

TEST(fmap, given_AnOptionalAndAMemberFunction_when_Mapping_should_ReturnTheMappedValueWrappedInAnOptional) {
    struct person {
        int id() const{ return 1;}
    };
    EXPECT_EQ(std::optional(1), std::optional{person{}} & &person::id);
    EXPECT_FALSE(std::optional<person>{} & &person::id);
}