#include <absent/absent.h>

#include <utility>
#include <gtest/gtest.h>

using namespace rvarago::absent;

TEST(combine_bind_fmap, given_anHierarchyOfPersonAddressAndZipCode_when_allAreNotEmpty_shouldReturnTheZipCode) {
    struct person{};
    struct address{};

    auto const find_person = []{ return std::optional{person{}}; };
    auto const find_address = [](auto const&){ return std::optional{address{}}; };
    auto const zip_code = [](auto const&){return "123";};

    EXPECT_EQ(std::optional{"123"}, find_person() >> find_address | zip_code);
}

TEST(combine_bind_fmap, given_anHierarchyOfPersonAddressAndZipCode_when_anyIsEmpty_shouldReturnAnEmptyZipCode) {
    struct person{};
    struct address{};

    auto const find_person = []{ return std::optional{person{}}; };
    auto const find_address = [](auto const&){ return std::optional{address{}}; };

    auto const find_person_empty = []() -> std::optional<person> { return std::nullopt; };
    auto const find_address_empty = [](auto const&) -> std::optional<address> { return std::nullopt; };

    auto const zip_code = [](auto const&){return "123";};

    EXPECT_FALSE(find_person() >> find_address_empty | zip_code);
    EXPECT_FALSE(find_person_empty() >> find_address | zip_code);
    EXPECT_FALSE(find_person_empty() >> find_address_empty | zip_code);
}

TEST(combine_bind_fmap, given_anHierarchyOfPersonAddressAndZipCodeAsMemberFunction_when_allAreNotEmpty_shouldReturnTheZipCode) {
    struct address{
        std::string zip_code() const {return "123";}
    };

    struct person{
        std::optional<address> find_address() const { return address{}; }
        std::optional<address> find_address_empty() const { return std::nullopt; }
    };

    auto const find_person = []{ return std::optional{person{}}; };
    auto const find_person_empty = []{ return std::optional<person>{}; };

    EXPECT_EQ(std::optional{"123"}, find_person() >> &person::find_address | &address::zip_code);
    EXPECT_FALSE(find_person() >> &person::find_address_empty | &address::zip_code);
    EXPECT_FALSE(find_person_empty() >> &person::find_address | &address::zip_code);
    EXPECT_FALSE(find_person_empty() >> &person::find_address_empty | &address::zip_code);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}