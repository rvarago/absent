#ifndef RVARAGO_ABSENT_SUPPORT_FROMVARIANT_H
#define RVARAGO_ABSENT_SUPPORT_FROMVARIANT_H

#include <optional>
#include <type_traits>
#include <variant>

namespace rvarago::absent {

/***
 * Given a nullable type N<A> (i.e. optional like object), and a variant<As....>
 * - When A is the same held by the variant: it should return a nullable N<A> wrapping the corresponding value.
 * - When *not*: it should return a new empty nullable N<A>.
 *
 * @param input an std::variant.
 * @return a new nullable containing the value wrapped by the variant, possibly empty.
 */
template <typename A, template <typename> typename Nullable = std::optional, typename... Rest>
constexpr auto from_variant(std::variant<Rest...> v) noexcept -> Nullable<A> {
    static_assert(std::disjunction_v<std::is_same<A, Rest>...>, "A is not a member type of the variant");
    if (std::holds_alternative<A>(v)) {
        return Nullable<A>{std::get<A>(std::move(v))};
    } else {
        return Nullable<A>{};
    }
}

}

#endif
