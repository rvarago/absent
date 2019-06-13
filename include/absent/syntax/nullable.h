#ifndef RVARAGO_ABSENT_NULLABLE_H
#define RVARAGO_ABSENT_NULLABLE_H

#include <utility>

namespace rvarago::absent::nullable {

    namespace syntax {

        /**
         * For custom types that have a different way to check if they are empty.
         */
        template<template<typename...> typename Nullable, typename A, typename... Rest>
        struct empty final {

            static constexpr auto _(Nullable<A, Rest...> const &input) -> bool {
                return !input.has_value();
            }

        };

        /**
         * For custom types that have a different way to access its wrapped value.
         */
        template<template<typename...> typename Nullable, typename A, typename... Rest>
        struct value final {

            static constexpr auto _(Nullable<A, Rest...> input) -> A {
                return input.value();
            }

        };

        /**
         * For custom types that have a different way to create new instances of itself from arguments.
         */
        template<template<typename...> typename Nullable, typename... Rest>
        struct make final {

            static constexpr auto _(Rest... args) -> Nullable<Rest...> {
                return Nullable<Rest...>{args...};
            }

        };

        /**
         * For custom types that have a different concept of empty state.
         */
        template<typename NullableIn, typename NullableOut>
        struct make_empty final {

            static constexpr auto _(NullableIn) -> NullableOut {
                return NullableOut{};
            }

        };

    }

    namespace instances {

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

            static constexpr auto _(Nullable<A, Rest...> input, Mapper fn) -> decltype(fn(std::declval<A>())) {
                using Result = decltype(fn(std::declval<A>()));
                if (syntax::empty<Nullable, A, Rest...>::_(input)) {
                    return syntax::make_empty<decltype(input), Result>::_(std::move(input));
                }
                return fn(syntax::value<Nullable, A, Rest...>::_(std::move(input)));
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

            static constexpr auto _(Nullable<A, Rest...> input, Mapper fn) -> Nullable<decltype(fn(std::declval<A>()))> {
                using ValueT = decltype(fn(std::declval<A>()));
                auto const flat_mapper = [&fn](auto value) {
                    return syntax::make<Nullable, ValueT>::_(fn(std::move(value)));
                };
                return binder<Nullable, decltype(flat_mapper), A, Rest...>::_(std::move(input), flat_mapper);
            }

        };
    }

}

#endif //RVARAGO_ABSENT_NULLABLE_H
