#ifndef RVARAGO_ABSENT_FROMVARIANT_H
#define RVARAGO_ABSENT_FROMVARIANT_H

#include <optional>
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
    if (auto const value = std::get_if<A>(&v); value) {
        return Nullable<A>{*value};
    }
    return Nullable<A>{};
}

}

#endif
