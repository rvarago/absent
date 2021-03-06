#ifndef RVARAGO_ABSENT_ADAPTERS_EITHER_FOREACH_H
#define RVARAGO_ABSENT_ADAPTERS_EITHER_FOREACH_H

#include "absent/adapters/either/either.h"

#include <functional>
#include <utility>

namespace rvarago::absent::adapters::either {

/***
 * Given an either<A, E> where E is a type that represents an error, and an unary function f: A -> void:
 * - When in error: it should do nothing.
 * - When *not* in error: it should apply the unary function to the input either's value only for its side-effect.
 *
 * @param input an either<A, B>.
 * @param action an unary function A -> void.
 */
template <typename UnaryFunction, typename A, typename E>
constexpr auto for_each(types::either<A, E> const &input,
                        UnaryFunction &&action) noexcept(noexcept(std::invoke(std::declval<UnaryFunction>(),
                                                                              std::declval<A>()))) -> void {
    if (auto const p = std::get_if<A>(&input); p) {
        std::invoke(std::forward<UnaryFunction>(action), *p);
    }
}

}

#endif
