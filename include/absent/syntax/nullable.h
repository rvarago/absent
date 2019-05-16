#ifndef RVARAGO_ABSENT_NULLABLE_H
#define RVARAGO_ABSENT_NULLABLE_H

namespace rvarago::absent::syntax::nullable {

    template <typename A, template <typename> typename Nullable>
    struct empty final {
        static constexpr auto _(Nullable<A> const& nullable) -> bool {
            return !nullable;
        }
    };

    template <typename A, template <typename> typename Nullable>
    struct value final {
        static constexpr auto _(Nullable<A> const &nullable) -> A {
            return *nullable;
        }
    };
}

#endif //RVARAGO_ABSENT_NULLABLE_H
