#ifndef RVARAGO_ABSENT_BOOST_OPTIONAL_H
#define RVARAGO_ABSENT_BOOST_OPTIONAL_H

#ifdef WITH_ADAPTER_FOR_BOOST_OPTIONAL

#include <boost/optional.hpp>

namespace rvarago::absent::syntax::nullable {

    template <typename Mapper, typename A>
    struct binder<boost::optional, Mapper, A> final {

        static constexpr decltype(auto) _(boost::optional<A> input, Mapper fn) {
            if (!input) {
                return decltype(fn(std::declval<A>())){};
            }
            return fn(std::move(input.value()));
        }

    };

}

#endif

#endif // RVARAGO_ABSENT_BOOST_OPTIONAL_H
