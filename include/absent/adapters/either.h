#ifndef RVARAGO_ABSENT_EITHER_H
#define RVARAGO_ABSENT_EITHER_H

#include <variant>

namespace rvarago::absent {

    namespace adapters {
        template<typename A, typename E>
        using either = std::variant<A, E>;
    }

    namespace nullable {

        namespace syntax {

            template <typename A, typename E>
            struct empty<std::variant, A, E> final {
                static constexpr auto _(std::variant<A, E> const &input) -> bool {
                    return !std::holds_alternative<A>(input);
                }
            };

            template <typename A, typename E>
            struct value<std::variant, A, E> final {
                static constexpr auto _(std::variant<A, E> input) -> A {
                    return std::get<A>(input);
                }
            };

            template <typename A, typename B, typename E>
            struct make_empty<std::variant<A, E>, std::variant<B, E>> final {
                static constexpr auto _(std::variant<A, E> input) -> std::variant<B, E> {
                    return std::variant<B, E>{std::get<E>(input)};
                }
            };
        }

        namespace instance {

            template <typename Mapper, typename A, typename E>
            struct fmapper<std::variant, Mapper, A, E> final {
                static constexpr decltype(auto) _(std::variant<A, E> input, Mapper fn) {
                    using ResultT = std::variant<decltype(fn(std::declval<A>())), E>;
                    auto const flat_mapper = [&fn](auto value) { return ResultT{fn(std::move(value))}; };
                    return binder<std::variant, decltype(flat_mapper), A, E>::_(std::move(input), flat_mapper);
                }
            };

        }

    }
}

#endif // RVARAGO_ABSENT_EITHER_H
