#ifndef RVARAGO_ABSENT_EITHER_H
#define RVARAGO_ABSENT_EITHER_H

#include <variant>

namespace rvarago::absent::syntax {

    template<typename A, typename E>
    using either = std::variant<A, E>;

    namespace nullable {

        template<typename Mapper, typename A, typename E>
        struct binder<std::variant, Mapper, A, E> final {

            static constexpr decltype(auto) bind(std::variant<A, E> input, Mapper fn) {
                using Result = decltype(fn(std::declval<A>()));
                if (!std::holds_alternative<A>(input)) {
                    return Result{std::get<E>(input)};
                }
                return fn(std::get<A>(input));
            }

        };

        template<typename Mapper, typename A, typename E>
        struct fmapper<std::variant, Mapper, A, E> final {

            static constexpr decltype(auto) fmap(std::variant<A, E> input, Mapper fn) {
                using Result = std::variant<decltype(fn(std::declval<A>())), E>;
                auto const flat_mapper = [&fn](auto value) { return Result{fn(std::move(value))}; };
                return binder<std::variant, decltype(flat_mapper), A, E>::bind(std::move(input), flat_mapper);
            }
        };
    }
}

#endif // RVARAGO_ABSENT_EITHER_H
