#ifndef RVARAGO_ABSENT_ADAPTERS_EITHER_TRANSFORM_H
#define RVARAGO_ABSENT_ADAPTERS_EITHER_TRANSFORM_H

#include "absent/adapters/either/either.h"

#include <utility>

namespace rvarago::absent::adapters::either {

/***
 * Given an either<A, E> where E is a type that represents an error, and an unary function f: A -> B:
 * - When in error: it should return a new either<B, E> in error wrapping the error value.
 * - When *not* in error: it should return a new either<B, E> wrapping the result of calling f with the input value of
 * type A.
 *
 * @param input an either<A, B>.
 * @param mapper an unary function A -> B.
 * @return a new either containing the mapped value of type B, possibly in error if input was also in error.
 */
template <typename A, typename E, typename UnaryFunction>
constexpr auto transform(types::either<A, E> input, UnaryFunction &&mapper) noexcept(
    noexcept(std::forward<UnaryFunction>(mapper)(std::declval<A>())))
    -> types::either<decltype(std::declval<UnaryFunction>()(std::declval<A>())), E> {
    using B = decltype(mapper(std::declval<A>()));
    if (std::holds_alternative<A>(std::move(input))) {
        return types::either<B, E>{std::forward<UnaryFunction>(mapper)(std::get<A>(std::move(input)))};
    }
    return types::either<B, E>{std::get<E>(std::move(input))};
}

/***
 * Infix version of transform.
 */
template <typename A, typename E, typename UnaryFunction>
constexpr auto operator|(types::either<A, E> input, UnaryFunction &&mapper) noexcept(
    noexcept(std::forward<UnaryFunction>(mapper)(std::declval<A>())))
    -> types::either<decltype(std::declval<UnaryFunction>()(std::declval<A>())), E> {
    return transform(std::move(input), std::forward<UnaryFunction>(mapper));
}

}

#endif
