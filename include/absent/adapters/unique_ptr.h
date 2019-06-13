#ifndef RVARAGO_ABSENT_UNIQUE_PTR_H
#define RVARAGO_ABSENT_UNIQUE_PTR_H

#include <memory>

namespace rvarago::absent::nullable::syntax {

    template <typename A, typename... Rest>
    struct empty<std::unique_ptr, A, Rest...> final {

        static constexpr auto _(std::unique_ptr<A, Rest...> const& input) -> bool {
            return !input;
        }

    };

    template <typename A, typename... Rest>
    struct value<std::unique_ptr, A, Rest...> final {

        static constexpr auto _(std::unique_ptr<A, Rest...> input) -> A {
            return *input.release();
        }

    };

    template <typename... Rest>
    struct make<std::unique_ptr, Rest...> final {

        static constexpr auto _(Rest... args) -> std::unique_ptr<Rest...> {
            return std::make_unique<Rest...>(args...);
        }

    };

}

#endif //RVARAGO_ABSENT_UNIQUE_PTR_H
