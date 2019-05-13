#ifndef RVARAGO_ABSENT_FOREACH_H
#define RVARAGO_ABSENT_FOREACH_H

#include "nullable.h"

namespace rvarago::absent {

    /***
     * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> void:
     * - When empty: it should do nothing.
     * - When *not* empty: it should apply the unary function to the input nullable's wrapped only for its side-effect.
     *
     * @param input any optional like object that can be checked against empty and provide access to its wrapped value.
     * @param fn an unary function A -> void.
     */
    template <typename A, template <typename> typename Nullable, typename Effect>
    constexpr auto foreach(Nullable<A> const& input, Effect fn) -> void {
        if (!nullable::empty(input)) {
            fn(nullable::value(input));
        }
    }


}

#endif //RVARAGO_ABSENT_FOREACH_H
