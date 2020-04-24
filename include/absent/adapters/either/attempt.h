#ifndef RVARAGO_ABSENT_ADAPTERS_EITHER_ATTEMPT_H
#define RVARAGO_ABSENT_ADAPTERS_EITHER_ATTEMPT_H

#include <exception>
#include <utility>

#include "absent/adapters/either/either.h"

namespace rvarago::absent::adapters::either {

/***
 * Given an either<A, BaseException> where E is a type that represents an error, and an nullary function f: () -> A that
 * may throw BaseException:
 * - When f throws: it should return an new invalid either<A, E> wrapping that threw exception.
 * - When f does not throw: it should return the value of type A returned by if f wrapped in an non-empty nullable.
 *
 * @param unsafe a nullary function () -> A that may throw.
 * @return a new nullable wrapping the value returned by unsafe, possibly invalid if unsafe threw.
 */
template <typename BaseException = std::exception, typename NullaryFunction>
auto attempt(NullaryFunction &&unsafe) -> types::either<decltype(std::declval<NullaryFunction>()()), BaseException> {
    using EitherA = types::either<decltype(unsafe()), BaseException>;
    try {
        return EitherA{std::forward<NullaryFunction>(unsafe)()};
    } catch (BaseException const &ex) {
        return EitherA{ex};
    }
}

}

#endif
