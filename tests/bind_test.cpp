#include <absent/bind.h>

#include <optional>
#include <string>

#include <gtest/gtest.h>

using namespace rvarago::absent;

TEST(bind, given_AnOptional_when_Empty_should_DoNothing) {
    auto const maybe_increment = [](auto const& a){ return std::optional{a + 1}; };

    std::optional<int> const empty_optional;

    EXPECT_FALSE(empty_optional >> maybe_increment);
    EXPECT_FALSE(empty_optional >> maybe_increment >> maybe_increment);
}

TEST(bind, given_AnOptional_when_NotEmpty_should_ReturnNewOptionalWithTheMappedValue) {
    auto const maybe_increment = [](auto const& a){ return std::optional{a + 1}; };

    std::optional<int> const some_zero{0};

    EXPECT_EQ(std::optional(1), some_zero >> maybe_increment);
    EXPECT_EQ(std::optional(2), some_zero >> maybe_increment >> maybe_increment);
    EXPECT_EQ(std::optional(3), some_zero >> maybe_increment >> maybe_increment >> maybe_increment);
}

TEST(bind, given_AnOptional_when_NotEmptyAndMappedToANewType_should_ReturnNewOptionalWithTheMappedValueOfTheNewType) {
    auto const maybe_string_to_int = [](auto const& a){ return std::optional{std::stoi(a)}; };
    auto const maybe_int_to_string = [](auto const& a){ return std::optional{std::to_string(a)}; };

    std::optional<std::string> const some_zero_as_string{"0"};

    EXPECT_EQ(std::optional(0), some_zero_as_string >> maybe_string_to_int);
    EXPECT_EQ(std::optional("0"), some_zero_as_string >> maybe_string_to_int >> maybe_int_to_string);
}

TEST(bind, given_AnOptional_when_NotEmptyAndThenEmpty_should_ReturnAnEmptyOptional) {
    auto const maybe_string_to_int = [](auto const& a){ return std::optional{std::stoi(a)}; };
    auto const to_empty_of_string = [](auto const&){ return std::optional<std::string>{}; };

    std::optional<std::string> const some_zero_as_string{"0"};

    EXPECT_FALSE(some_zero_as_string >> maybe_string_to_int >> to_empty_of_string);
}

TEST(bind, given_AnOptionalAndAMemberFunction_when_Mapping_should_ReturnTheMappedValueWrappedInAnFlattenOptional) {
    struct person {
        std::optional<int> id() const{ return 1;}
        std::optional<int> id_empty() const{ return std::optional<int>{};}
    };

    EXPECT_EQ(std::optional{1}, std::optional{person{}} >> &person::id);
    EXPECT_FALSE(std::optional<person>{} >> &person::id);
    EXPECT_FALSE(std::optional{person{}} >> &person::id_empty);
    EXPECT_FALSE(std::optional<person>{} >> &person::id_empty);
}
