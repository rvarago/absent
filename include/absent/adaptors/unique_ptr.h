#ifndef RVARAGO_ABSENT_UNIQUE_PTR_H
#define RVARAGO_ABSENT_UNIQUE_PTR_H

#include <memory>

namespace rvarago::absent::syntax::nullable {

    template <typename Mapper, typename A, typename... Rest>
    struct binder<std::unique_ptr, Mapper, A, Rest...> final {

        static constexpr decltype(auto) bind(std::unique_ptr<A, Rest...> input, Mapper fn) {
            using Result = decltype(fn(std::declval<A>()));
            if (!input) {
                return Result{};
            }
            return fn(*input.release());
        }

    };

    template <typename Mapper, typename A, typename... Rest>
    struct fmapper<std::unique_ptr, Mapper, A, Rest...> final {

        static constexpr decltype(auto) fmap(std::unique_ptr<A, Rest...> input, Mapper fn) {
            using Result = decltype(fn(std::declval<A>()));
            auto const flat_mapper = [&fn](auto value){ return std::make_unique<Result>(fn(std::move(value))); };
            return binder<std::unique_ptr, decltype(flat_mapper), A, Rest...>::bind(std::move(input), flat_mapper);
        }

    };

}

#endif //RVARAGO_ABSENT_UNIQUE_PTR_H
