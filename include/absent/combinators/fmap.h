#ifndef RVARAGO_ABSENT_FMAP_H
#define RVARAGO_ABSENT_FMAP_H

#include "absent/syntax/nullable.h"
#include "absent/syntax/member.h"

#include <functional>
#include <utility>

namespace rvarago::absent {

    /***
     * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> B:
     * - When empty: it should return a new empty nullable N<B>.
     * - When *not* empty: it should return a value generated by applying the unary mapping function to the input
     * nullable's wrapped in a brand new nullable N<B>.
     *
     * @param input any optional like object that can be checked against empty and provide access to its wrapped value.
     * @param fn an unary function A -> B.
     * @return a new nullable containing the mapped value of type B, possibly empty if input is also empty.
     */
    template <template <typename...> typename Nullable, typename Mapper, typename A, typename... Rest>
    constexpr decltype(auto) fmap(Nullable<A, Rest...> input, Mapper fn) {
        return nullable::instances::fmapper<Nullable, Mapper, A, Rest...>::_(std::move(input), fn);
    }

    /***
     * The same as fmap but for a member function that has to be const and parameterless.
     */
    template <template <typename...> typename Nullable, typename A, typename B, typename... Rest>
    constexpr decltype(auto) fmap(Nullable<A, Rest...> input, syntax::member::Mapper<const A, B> fn) {
        return fmap(std::move(input), [&fn](auto value){ return std::invoke(fn, std::move(value)); });
    }

    /***
     * Infix version of fmap.
     */
    template <template <typename...> typename Nullable, typename Mapper, typename A, typename... Rest>
    constexpr decltype(auto) operator|(Nullable<A, Rest...> input, Mapper fn) {
        return fmap(std::move(input), fn);
    }

    /**
     * Infix version of fmap for a member function.
     */
    template <template <typename...> typename Nullable, typename A, typename B, typename... Rest>
    constexpr decltype(auto) operator|(Nullable<A, Rest...> input, syntax::member::Mapper<const A, B> fn) {
        return fmap(std::move(input), fn);
    }

}

#endif //RVARAGO_ABSENT_FMAP_H
