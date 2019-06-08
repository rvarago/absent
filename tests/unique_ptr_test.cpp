#include <absent/absent.h>
#include <absent/adapters/unique_ptr.h>

#include <gtest/gtest.h>

using namespace rvarago::absent;

TEST(unique_ptr, given_andUniquePtr_when_InvokeAbsent_shouldMoveItAndLeaveItNullAfterwards) {
    auto zero = std::make_unique<int>(0);
    auto const one = std::move(zero) | [](auto value){ return value + 1; };

    EXPECT_FALSE(zero);
    EXPECT_TRUE(one);
    EXPECT_EQ(1, *one);
}

TEST(unique_ptr, given_andUniquePtr_when_notEmpty_shouldApplyForeachToIncrementCounter) {
    int counter = 0;
    auto const add_to_counter = [&counter](auto const& a){ counter += a; };

    auto one = std::make_unique<int>(1);
    foreach(std::move(one), add_to_counter);

    EXPECT_FALSE(one);
    EXPECT_EQ(1, counter);
}

TEST(unique_ptr, given_anUniquePtr_when_notEmpty_shouldReturnEmptyUniquePtr) {
    struct person{};
    struct address{};

    auto const find_person_empty = []() -> std::unique_ptr<person> { return nullptr; };
    auto const find_address = [](auto const&){ return std::make_unique<address>(); };
    auto const zip_code = [](auto const&){return "123";};

    EXPECT_FALSE(find_person_empty() >> find_address | zip_code);
}

TEST(unique_ptr, given_anUniquePtr_when_notNotEmpty_shouldReturnNewUniquePtr) {
    struct person{};
    struct address{};

    auto const find_person = []{ return std::make_unique<person>(); };
    auto const find_address = [](auto const&){ return std::make_unique<address>(); };
    auto const zip_code = [](auto const&){return "123";};

    EXPECT_EQ("123", *(find_person() >> find_address | zip_code));
}