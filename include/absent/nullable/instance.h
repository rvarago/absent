#ifndef RVARAGO_ABSENT_INSTANCE_H
#define RVARAGO_ABSENT_INSTANCE_H

#include <utility>
#include <absent/nullable/syntax.h>

namespace rvarago::absent::nullable::instance {

    /**
     * This is one of the fundamental building blocks of absent, and combinators are built on top of it.
     *
     * For custom types that have an interface not adhering to the default one expected by absent should provide
     * specialization of this type in order to be interoperable.
     *
     * @tparam Nullable nullable type N
     * @tparam Mapper function that maps from A => N<B>
     * @tparam A wrapped type
     */
    template<template<typename...> typename Nullable, typename Mapper, typename A, typename... Rest>
    struct binder final {
        static constexpr auto _(Nullable<A, Rest...> input, Mapper fn) noexcept -> decltype(fn(std::declval<A>())) {
            using ResultT = decltype(fn(std::declval<A>()));
            if (syntax::empty<Nullable, A, Rest...>::_(input)) {
                return syntax::make_empty<Nullable<A, Rest...>, ResultT>::_(std::move(input));
            }
            auto const value = syntax::value<Nullable, A, Rest...>::_(std::move(input));
            return fn(std::move(value));
        }
    };

    /**
     * For custom types that have an interface not adhering to the default one expected by absent should provide
     * specialization of this type in order to be interoperable if providing one for binder is not enough.
     *
     * @tparam Nullable nullable type N
     * @tparam Mapper function that maps from A => B
     * @tparam A wrapped type
     */
    template<template<typename...> typename Nullable, typename Mapper, typename A, typename... Rest>
    struct fmapper final {
        static constexpr auto _(Nullable<A, Rest...> input, Mapper fn) noexcept -> Nullable<decltype(fn(std::declval<A>())), Rest...> {
            using ValueT = decltype(fn(std::declval<A>()));
            if (syntax::empty<Nullable, A, Rest...>::_(input)) {
                return syntax::make_empty<Nullable<A, Rest...>, Nullable<ValueT, Rest...>>::_(std::move(input));
            }
            auto const value = syntax::value<Nullable, A, Rest...>::_(std::move(input));
            return syntax::make<Nullable, ValueT, Rest...>::_(fn(std::move(value)));
        }
    };

}


#endif //RVARAGO_ABSENT_INSTANCE_H
