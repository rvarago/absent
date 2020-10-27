#ifndef RVARAGO_ABSENT_ADAPTERS_EITHER_ANDTHEN_H
#define RVARAGO_ABSENT_ADAPTERS_EITHER_ANDTHEN_H

#include "absent/adapters/either/either.h"

#include <utility>

namespace rvarago::absent::adapters::either {

/***
 * Given an either<A, E> where E is a type that represents an error, and an unary function, and an unary function f: A
 * -> either<B, E>:
 * - When in error: it should return a new either<B, E> in error wrapping the error value.
 * - When *not* in error: it should return a new either<B, E> generated by applying the unary mapping function to the
 * input value of type A, already wrapped in a either<B, E>.
 *
 * @param input an either<A, B>.
 * @param mapper an unary function A -> either<B, E>.
 * @return a new either containing the mapped value of type B, possibly in error if input was also in error.
 */
template <typename A, typename E, typename UnaryFunction>
constexpr auto and_then(types::either<A, E> input,
                        UnaryFunction &&mapper) noexcept(noexcept(std::declval<UnaryFunction>()(std::declval<A>())))
    -> decltype(std::declval<UnaryFunction>()(std::declval<A>())) {
    using EitherB = decltype(mapper(std::declval<A>()));
    if (std::holds_alternative<A>(input)) {
        return std::forward<UnaryFunction>(mapper)(std::get<A>(std::move(input)));
    }
    return EitherB{std::get<E>(std::move(input))};
}

/***
 * Infix version of and_then.
 */
template <typename A, typename E, typename UnaryFunction>
constexpr auto operator>>(types::either<A, E> input,
                          UnaryFunction &&mapper) noexcept(noexcept(std::declval<UnaryFunction>()(std::declval<A>())))
    -> decltype(std::declval<UnaryFunction>()(std::declval<A>())) {
    return and_then(std::move(input), std::forward<UnaryFunction>(mapper));
}

}

#endif
