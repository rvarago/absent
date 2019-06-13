#ifndef RVARAGO_ABSENT_EVAL_H
#define RVARAGO_ABSENT_EVAL_H

#include "absent/nullable/syntax.h"

#include <utility>

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
    template <template <typename...> typename Nullable, typename Fallback, typename A, typename... Rest>
    constexpr decltype(auto) eval(Nullable<A, Rest...> input, Fallback fn) {
        if (nullable::syntax::empty<Nullable, A, Rest...>::_(input)) {
            return fn();
        }
        return nullable::syntax::value<Nullable, A, Rest...>::_(std::move(input));
    }

}

#endif //RVARAGO_ABSENT_EVAL_H
