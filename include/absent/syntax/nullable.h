#ifndef RVARAGO_ABSENT_NULLABLE_H
#define RVARAGO_ABSENT_NULLABLE_H

#include <utility>

namespace rvarago::absent::syntax::nullable {

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
    template <template <typename...> typename Nullable, typename Mapper, typename A, typename... Rest>
    struct binder final {

        static constexpr auto bind(Nullable<A, Rest...> input, Mapper fn) -> decltype(fn(std::declval<A>())) {
            if (!input.has_value()) {
                return decltype(fn(std::declval<A>())){};
            }
            return fn(std::move(input.value()));
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
    template <template <typename...> typename Nullable, typename Mapper, typename A, typename... Rest>
    struct fmapper final {

        static constexpr auto fmap(Nullable<A, Rest...> input, Mapper fn) -> Nullable<decltype(fn(std::declval<A>()))> {
            using Result = Nullable<decltype(fn(std::declval<A>()))>;
            auto const flat_mapper = [&fn](auto value){ return Result{fn(std::move(value))}; };
            return binder<Nullable, decltype(flat_mapper), A, Rest...>::bind(std::move(input), flat_mapper);
        }

    };

}

#endif //RVARAGO_ABSENT_NULLABLE_H
