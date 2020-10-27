#ifndef RVARAGO_ABSENT_EVAL_H
#define RVARAGO_ABSENT_EVAL_H

#include <functional>
#include <utility>

namespace rvarago::absent {

/***
 * Given a nullable type N<A> (i.e. optional-like object), and a nullary function f: () -> A:
 * - When empty: it should evaluate the function f that returns a fallback instance of type A.
 * - When *not* empty: it should return the wrapped value of type A.
 *
 * @param input a nullable N<A>.
 * @param fallback a nullary function () -> A.
 * @return the wrapped value inside the nullable or the result of fallback if the nullable is empty.
 */
template <template <typename> typename Nullable, typename NullaryFunction, typename A>
constexpr auto eval(Nullable<A> const &input,
                    NullaryFunction &&fallback) noexcept(noexcept(std::invoke(std::declval<NullaryFunction>()))) -> A {
    if (!input) {
        return std::invoke(std::forward<NullaryFunction>(fallback));
    } else {
        return *input;
    }
}

}

#endif
