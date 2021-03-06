#ifndef RVARAGO_ABSENT_ADAPTERS_EITHER_TRANSFORM_H
#define RVARAGO_ABSENT_ADAPTERS_EITHER_TRANSFORM_H

#include "absent/adapters/either/either.h"

#include <functional>
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
constexpr auto transform(types::either<A, E> const &input,
                         UnaryFunction &&mapper) noexcept(noexcept(std::invoke(std::forward<UnaryFunction>(mapper),
                                                                               std::declval<A>())))
    -> types::either<decltype(std::invoke(std::declval<UnaryFunction>(), std::declval<A>())), E> {
    using B = decltype(std::invoke(mapper, std::declval<A>()));
    if (auto const p = std::get_if<A>(&input); p) {
        return types::either<B, E>{std::invoke(std::forward<UnaryFunction>(mapper), *p)};
    } else {
        return types::either<B, E>{std::get<E>(input)};
    }
}

/***
 * Infix version of transform.
 */
template <typename A, typename E, typename UnaryFunction>
constexpr auto operator|(types::either<A, E> const &input,
                         UnaryFunction &&mapper) noexcept(noexcept(std::invoke(std::forward<UnaryFunction>(mapper),
                                                                               std::declval<A>())))
    -> types::either<decltype(std::invoke(std::declval<UnaryFunction>(), std::declval<A>())), E> {
    return transform(input, std::forward<UnaryFunction>(mapper));
}

}

#endif
