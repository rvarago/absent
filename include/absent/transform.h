#ifndef RVARAGO_ABSENT_TRANSFORM_H
#define RVARAGO_ABSENT_TRANSFORM_H

#include <utility>

namespace rvarago::absent {

/***
 * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> B:
 * - When empty: it should return a new empty nullable N<B>.
 * - When *not* empty: it should return a nullable N<B> wrapping the result of calling f with the input value of type A.
 *
 * @param input a nullable N<A>.
 * @param mapper an unary function A -> B.
 * @return a new nullable containing the mapped value of type B, possibly empty if input was also empty.
 */
template <template <typename> typename Nullable, typename A, typename UnaryFunction>
constexpr auto transform(Nullable<A> input,
                         UnaryFunction &&mapper) noexcept(noexcept(std::declval<UnaryFunction>()(std::declval<A>())))
    -> Nullable<decltype(std::declval<UnaryFunction>()(std::declval<A>()))> {
    using B = decltype(mapper(std::declval<A>()));
    if (!input) {
        return Nullable<B>{};
    }
    return Nullable<B>{std::forward<UnaryFunction>(mapper)(*std::move(input))};
}

/***
 * Infix version of transform.
 */
template <template <typename> typename Nullable, typename A, typename UnaryFunction>
constexpr auto operator|(Nullable<A> input,
                         UnaryFunction &&mapper) noexcept(noexcept(std::declval<UnaryFunction>()(std::declval<A>())))
    -> Nullable<decltype(std::declval<UnaryFunction>()(std::declval<A>()))> {
    return transform(std::move(input), std::forward<UnaryFunction>(mapper));
}

}

#endif
