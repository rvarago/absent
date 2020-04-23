#ifndef RVARAGO_ABSENT_TRANSFORM_H
#define RVARAGO_ABSENT_TRANSFORM_H

#include <functional>

#include "absent/support/member.h"

namespace rvarago::absent {

/***
 * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> B:
 * - When empty: it should return a new empty nullable N<B>.
 * - When *not* empty: it should return a nullable N<B> wrapping the result of calling f with the input value of type A.
 *
 * @param input a nullable N<A>.
 * @param mapper an unary function A -> B.
 * @return a new nullable containing the mapped value of type B, possibly empty if input was also empty.
 */
template <template <typename> typename Nullable, typename A, typename UnaryFunction>
constexpr auto transform(Nullable<A> const &input, UnaryFunction mapper) noexcept
    -> Nullable<decltype(mapper(std::declval<A>()))> {
    using NullableB = Nullable<decltype(mapper(std::declval<A>()))>;
    if (!input) {
        return NullableB{};
    }
    return NullableB{mapper(*input)};
}

/***
 * Infix version of transform.
 */
template <template <typename> typename Nullable, typename A, typename UnaryFunction>
constexpr auto operator|(Nullable<A> const &input, UnaryFunction mapper) noexcept
    -> Nullable<decltype(mapper(std::declval<A>()))> {
    return transform(input, mapper);
}

/***
 * The same as transform but for a member function that has to be const and parameterless.
 */
template <template <typename> typename Nullable, typename A, typename B>
constexpr auto transform(Nullable<A> const &input, support::member_mapper<const A, B> mapper) noexcept -> Nullable<B> {
    return transform(input, [&mapper](auto const &value) { return std::invoke(mapper, value); });
}

/**
 * Infix version of transform for a member function.
 */
template <template <typename> typename Nullable, typename A, typename B>
constexpr auto operator|(Nullable<A> const &input, support::member_mapper<const A, B> mapper) noexcept -> Nullable<B> {
    return transform(input, mapper);
}

}

#endif
