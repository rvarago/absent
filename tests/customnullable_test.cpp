#include <absent/absent.h>

#include <utility>
#include <gtest/gtest.h>

using namespace rvarago::absent;

namespace {
    template<typename T>
    struct custom_nullable final {
        explicit custom_nullable() = default;
        explicit custom_nullable(T val) : value{std::move(val)}, has_value(true) {}

        T value;
        bool const has_value{false};
    };

    template <typename T>
    bool operator==(custom_nullable<T> const& rhs, custom_nullable<T> const& lhs) {
        return rhs.has_value && lhs.has_value && rhs.value == rhs.value;
    }
}

namespace rvarago::absent::nullable::syntax {

    template <typename A>
    struct empty<custom_nullable, A> final {

        static constexpr auto _(custom_nullable<A> const& input) -> bool {
            return !input.has_value;
        }

    };

    template <typename A>
    struct value<custom_nullable, A> final {

        static constexpr auto _(custom_nullable<A> input) -> A {
            return std::move(input.value);
        }

    };

}

TEST(custom_nullable, given_aCustomNullable_when_notInvokeAbsentOperations_shouldUseSpecializations) {
    struct person{};
    struct address{};

    auto const find_person = []{ return custom_nullable{person{}}; };
    auto const find_person_empty = []{ return custom_nullable<person>{}; };

    auto const find_address = [](auto const&){ return custom_nullable{address{}}; };
    auto const zip_code = [](auto const&){return "123";};

    EXPECT_EQ(custom_nullable{"123"}, find_person() >> find_address | zip_code);
    EXPECT_FALSE((find_person_empty() >> find_address | zip_code).has_value);
}