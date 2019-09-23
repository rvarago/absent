#ifndef RVARAGO_ABSENT_EVAL_H
#define RVARAGO_ABSENT_EVAL_H

#include "absent/nullable/syntax.h"

namespace rvarago::absent {

    /***
     * Given a nullable type N<A> (i.e. optional like object), and a nullary function f: () -> A:
     * - When empty: it should evaluates the function f, returning an instance of A.
     * - When *not* empty: it should return the value of type A wrapped in the nullable.
     *
     * @param input any optional like object that can be checked against empty and provide access to its wrapped value.
     * @param fn a nullary function () -> A.
     * @return the wrapped value inside the nullable or the result of fn if the nullable is empty.
     */
    template <template <typename...> typename Nullable, typename NullaryFunction, typename A, typename... Rest>
    constexpr decltype(auto) eval(Nullable<A, Rest...> const& input, NullaryFunction fallback) noexcept {
        if (nullable::syntax::empty<Nullable, A, Rest...>::_(input)) {
            return fallback();
        }
        return nullable::syntax::value<Nullable, A, Rest...>::_(input);
    }

}

#endif //RVARAGO_ABSENT_EVAL_H
