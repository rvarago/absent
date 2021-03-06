#ifndef RVARAGO_ABSENT_ANDTHEN_H
#define RVARAGO_ABSENT_ANDTHEN_H

#include <functional>
#include <utility>

namespace rvarago::absent {

/***
 * Given a nullable type N<A> (i.e. optional-like object), and an unary function f: A -> N<B>:
 * - When empty: it should return a new empty nullable N<B>.
 * - When *not* empty: it should return a nullable N<B> generated by applying the unary mapping function to the input
 * nullable's value, already wrapped in a nullable N<B>.
 *
 * @param input a nullable N<A>.
 * @param mapper an unary function A -> N<B>.
 * @return a new nullable generated by mapper, possibly empty if input was also empty.
 */
template <template <typename> typename Nullable, typename UnaryFunction, typename A>
constexpr auto and_then(Nullable<A> const &input,
                        UnaryFunction &&mapper) noexcept(noexcept(std::invoke(std::declval<UnaryFunction>(),
                                                                              std::declval<A>())))
    -> decltype(std::invoke(std::declval<UnaryFunction>(), std::declval<A>())) {
    using NullableB = decltype(std::invoke(mapper, std::declval<A>()));
    if (!input) {
        return NullableB{};
    } else {
        return std::invoke(std::forward<UnaryFunction>(mapper), *input);
    }
}

/***
 * Infix version of and_then.
 */
template <template <typename> typename Nullable, typename UnaryFunction, typename A>
constexpr auto operator>>(Nullable<A> const &input,
                          UnaryFunction &&mapper) noexcept(noexcept(std::invoke(std::declval<UnaryFunction>(),
                                                                                std::declval<A>())))
    -> decltype(std::invoke(std::declval<UnaryFunction>(), std::declval<A>())) {
    return and_then(input, std::forward<UnaryFunction>(mapper));
}

}

#endif
