#ifndef RVARAGO_ABSENT_SYNTAX_H
#define RVARAGO_ABSENT_SYNTAX_H

#include <utility>

namespace rvarago::absent::nullable::syntax {

    /**
     * For custom types that have a different way to check if they are empty.
     */
    template <template<typename...> typename Nullable, typename A, typename... Rest>
    struct empty {
        static constexpr auto _(Nullable<A, Rest...> const& input) noexcept -> bool {
            return !input;
        }
    };

    /**
     * For custom types that have a different way to access its wrapped value.
     */
    template <template<typename...> typename Nullable, typename A, typename... Rest>
    struct value {
        static constexpr auto _(Nullable<A, Rest...> const& input) noexcept -> A {
            return *input;
        }
    };

    /**
     * For custom types that have a different way to create new instances of itself from arguments.
     */
    template <template<typename...> typename Nullable, typename... Rest>
    struct make {
        template <typename... Args>
        static constexpr auto _(Args&&... args) noexcept -> Nullable<Rest...> {
            return Nullable<Rest...>{std::forward<Args>(args)...};
        }
    };

    /**
     * For custom types that have a different concept of empty state.
     */
    template <typename Nullable>
    struct make_empty {
        template <typename... Args>
        static constexpr auto _(Args&&...) noexcept -> Nullable {
            return Nullable{};
        }
    };

}

#endif //RVARAGO_ABSENT_SYNTAX_H
