#ifndef RVARAGO_ABSENT_ATTEMPT_H
#define RVARAGO_ABSENT_ATTEMPT_H

#include <exception>
#include <functional>
#include <optional>
#include <utility>

namespace rvarago::absent {

/***
 * Given a nullable type N<A> (i.e. optional-like object), and an nullary function f: () -> A that may throw
 * BaseException:
 * - When f throws: it should return a new empty nullable N<A>.
 * - When f does not throw: it should return the value of type A returned by if f wrapped in an non-empty nullable.
 *
 * @param unsafe a nullary function () -> A that may throw.
 * @return a new nullable wrapping the value returned by unsafe, possibly empty if unsafe threw.
 */
template <typename BaseException = std::exception, template <typename> typename Nullable = std::optional,
          typename NullaryFunction>
auto attempt(NullaryFunction &&unsafe) -> Nullable<decltype(std::invoke(std::declval<NullaryFunction>()))> {
    using NullableA = Nullable<decltype(std::invoke(unsafe))>;
    try {
        return NullableA{std::invoke(std::forward<NullaryFunction>(unsafe))};
    } catch (BaseException const &) {
        return NullableA{};
    }
}

}

#endif
