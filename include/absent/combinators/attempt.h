#ifndef RVARAGO_ABSENT_ATTEMPT_H
#define RVARAGO_ABSENT_ATTEMPT_H

#include <exception>

#include "absent/nullable/syntax.h"

namespace rvarago::absent {

template <template <typename...> typename Nullable, typename... Rest>
struct attempt {

    template <typename BaseException = std::exception, typename NullaryFunction>
    static auto or_catch(NullaryFunction unsafe) -> Nullable<decltype(unsafe()), Rest...> {
        using namespace nullable::syntax;
        using ValueT = decltype(unsafe());
        try {
            return make<Nullable, ValueT, Rest...>::_(unsafe());
        } catch (BaseException const &ex) {
            return make_empty<Nullable<ValueT, Rest...>>::_(ex);
        }
    }
};

}

#endif
