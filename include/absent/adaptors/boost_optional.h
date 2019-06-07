#ifndef RVARAGO_ABSENT_BOOST_OPTIONAL_H
#define RVARAGO_ABSENT_BOOST_OPTIONAL_H

#ifdef WITH_ADAPTOR_FOR_BOOST_OPTIONAL

#include <boost/optional.hpp>

namespace rvarago::absent::syntax::nullable {

    template <typename A>
    struct empty<boost::optional, A> final {
        static constexpr auto _(boost::optional<A> const& nullable) -> bool {
            return !nullable;
        }
    };

}
#endif

#endif // RVARAGO_ABSENT_BOOST_OPTIONAL_H
