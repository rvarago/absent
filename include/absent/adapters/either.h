#ifndef RVARAGO_ABSENT_EITHER_H
#define RVARAGO_ABSENT_EITHER_H

#include <variant>

namespace rvarago::absent {

    namespace adapters {
        template<typename A, typename E>
        using either = std::variant<A, E>;
    }

    namespace nullable::syntax {

            template <typename A, typename E>
            struct empty<std::variant, A, E> final {
                static constexpr auto _(std::variant<A, E> const &input) noexcept -> bool {
                    return !std::holds_alternative<A>(input);
                }
            };

            template <typename A, typename E>
            struct value<std::variant, A, E> final {
                static constexpr auto _(std::variant<A, E> const& input) noexcept -> A {
                    return std::get<A>(input);
                }
            };

            template <typename B, typename E>
            struct make_empty<std::variant<B, E>> final {
                template <typename A>
                static constexpr auto _(std::variant<A, E> const& input) noexcept -> std::variant<B, E> {
                    return std::variant<B, E>{std::get<E>(input)};
                }

                static constexpr auto _(E const& right_choice) noexcept -> std::variant<B, E> {
                    return std::variant<B, E>{right_choice};
                }
            };
        }
}

#endif // RVARAGO_ABSENT_EITHER_H
