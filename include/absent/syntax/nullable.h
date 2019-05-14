#ifndef RVARAGO_ABSENT_NULLABLE_H
#define RVARAGO_ABSENT_NULLABLE_H

namespace rvarago::absent::syntax::nullable {

    template <typename Nullable>
    constexpr auto empty(Nullable const& nullable) -> bool {
        return !nullable;
    }

    template <typename A, template <typename> typename Nullable>
    constexpr auto value(Nullable<A> const& nullable) ->  A {
        return *nullable;
    }
}

#endif //RVARAGO_ABSENT_NULLABLE_H
