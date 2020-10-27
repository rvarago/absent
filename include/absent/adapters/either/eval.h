#ifndef RVARAGO_ABSENT_ADAPTERS_EITHER_EVAL_H
#define RVARAGO_ABSENT_ADAPTERS_EITHER_EVAL_H

#include "absent/adapters/either/either.h"

#include <utility>

namespace rvarago::absent::adapters::either {

/***
 * Given an either<A, E> where E is a type that represents an error, and a nullary function f: () -> A:
 * - When in error: it should evaluate the function f that returns a fallback instance of type A.
 * - When *not* in error: it should return the wrapped value of type A.
 *
 * @param input an either<A, E>.
 * @param fallback a nullary function () -> A.
 * @return the wrapped value inside the either or the result of fallback if the either is in error.
 */
template <typename NullaryFunction, typename A, typename E>
constexpr auto eval(types::either<A, E> input,
                    NullaryFunction &&fallback) noexcept(noexcept(std::declval<NullaryFunction>()())) -> A {
    if (!std::holds_alternative<A>(input)) {
        return std::forward<NullaryFunction>(fallback)();
    }
    return std::get<A>(std::move(input));
}

}

#endif
