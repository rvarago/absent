#ifndef RVARAGO_ABSENT_FOREACH_H
#define RVARAGO_ABSENT_FOREACH_H

#include <utility>

namespace rvarago::absent {

/***
 * Given a nullable type N<A> (i.e. optional-like object), and an unary function f: A -> void:
 * - When empty: it should do nothing.
 * - When *not* empty: it should apply the unary function to the input nullable's value only for its side-effect.
 *
 * @param input a nullable N<A>.
 * @param action an unary function A -> void.
 */
template <template <typename> typename Nullable, typename UnaryFunction, typename A>
constexpr auto for_each(Nullable<A> const &input, UnaryFunction &&action) noexcept -> void {
    if (!input) {
        return;
    }
    std::forward<UnaryFunction>(action)(*input);
}

}

#endif
