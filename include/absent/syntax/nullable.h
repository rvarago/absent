#ifndef RVARAGO_ABSENT_NULLABLE_H
#define RVARAGO_ABSENT_NULLABLE_H

namespace rvarago::absent::syntax::nullable {

    template <template <typename> typename Nullable, typename A>
    struct empty final {
        static constexpr auto _(Nullable<A> const& nullable) -> bool {
            return !nullable.has_value();
        }
    };

    template <template <typename> typename Nullable, typename A>
    struct value final {
        static constexpr auto _(Nullable<A> const &nullable) -> A {
            return nullable.value();
        }
    };
}

#endif //RVARAGO_ABSENT_NULLABLE_H
