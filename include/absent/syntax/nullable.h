#ifndef RVARAGO_ABSENT_NULLABLE_H
#define RVARAGO_ABSENT_NULLABLE_H

#include <utility>

namespace rvarago::absent::syntax {

    /**
     * This is one of the fundamental building blocks of absent, and combinator are built on top of it.
     *
     * For custom types that have an interface not adhering to the default one expected by absent should provide
     * specialization of this type in order to be interoperable.
     *
     * @tparam Nullable nullable type N
     * @tparam Mapper function that maps from A => N[B]
     * @tparam A wrapped type
     */
    template <template <typename> typename Nullable, typename Mapper, typename A>
    struct nullable final {

        /***
         * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> N<B>:
         * - When empty: it should return a new empty nullable N<B>.
         * - When *not* empty: it should return a value generated by applying the unary mapping function to the input
         * nullable's which is already wrapped a nullable N<B>.
         *
         * @param input any optional like object that can be checked against empty and provide access to its wrapped value.
         * @param fn an unary function A -> N<B>.
         * @return a new nullable generated by fn, possibly empty if input is also empty.
         */
        static constexpr auto bind(Nullable<A> input, Mapper fn) -> decltype(fn(std::declval<A>())) {
            if (!input.has_value()) {
                return decltype(fn(std::declval<A>())){};
            }
            return fn(std::move(input.value()));
        }

    };

}

#endif //RVARAGO_ABSENT_NULLABLE_H
