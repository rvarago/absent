#include <absent/absent.h>

#include <optional>
#include <utility>

#include <gtest/gtest.h>

#define WITH_ADAPTER_FOR_BOOST_OPTIONAL 1

#include <absent/adapters/boost_optional.h>

#include <boost/optional.hpp>
using namespace rvarago::absent;

namespace {

    struct person final {};
    struct address final {};

    TEST(combine_bind_fmap, given_anHierarchyOfPersonAddressAndZipCode_when_allAreNotEmpty_shouldReturnTheZipCode) {
        auto const find_person = [] { return std::optional{person{}}; };
        auto const find_address = [](auto const &) { return std::optional{address{}}; };
        auto const zip_code = [](auto const &) { return 42; };

        EXPECT_EQ(std::optional{42}, find_person() >> find_address | zip_code);
    }

    TEST(combine_bind_fmap, given_anHierarchyOfPersonAddressAndZipCode_when_anyIsEmpty_shouldReturnAnEmptyZipCode) {
        auto const find_person = [] { return std::optional{person{}}; };
        auto const find_address = [](auto const &) { return std::optional{address{}}; };

        auto const find_person_empty = []() -> std::optional<person> { return std::nullopt; };
        auto const find_address_empty = [](auto const &) -> std::optional<address> { return std::nullopt; };

        auto const zip_code = [](auto const &) { return 42; };

        EXPECT_FALSE(find_person() >> find_address_empty | zip_code);
        EXPECT_FALSE(find_person_empty() >> find_address | zip_code);
        EXPECT_FALSE(find_person_empty() >> find_address_empty | zip_code);
    }

    TEST(combine_bind_fmap, given_anHierarchyOfPersonAddressAndZipCodeAsMemberFunction_when_allAreNotEmpty_shouldReturnTheZipCode) {

        struct address {
            int zip_code() const { return 42; }
        };

        struct person {
            std::optional<address> find_address() const { return address{}; }
            std::optional<address> find_address_empty() const { return std::nullopt; }
        };

        auto const find_person = [] { return std::optional{person{}}; };
        auto const find_person_empty = [] { return std::optional<person>{}; };

        EXPECT_EQ(std::optional{42}, find_person() >> &person::find_address | &address::zip_code);
        EXPECT_FALSE(find_person() >> &person::find_address_empty | &address::zip_code);
        EXPECT_FALSE(find_person_empty() >> &person::find_address | &address::zip_code);
        EXPECT_FALSE(find_person_empty() >> &person::find_address_empty | &address::zip_code);
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}