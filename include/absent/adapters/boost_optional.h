#ifndef RVARAGO_ABSENT_BOOST_OPTIONAL_H
#define RVARAGO_ABSENT_BOOST_OPTIONAL_H

#ifdef WITH_ADAPTER_FOR_BOOST_OPTIONAL

#include <boost/optional.hpp>

namespace rvarago::absent::nullable::syntax {

    template <typename A>
    struct empty<boost::optional, A> final {
        static constexpr auto _(boost::optional<A> const& input) -> bool {
            return !input;
        }
    };

}

#endif // WITH_ADAPTER_FOR_BOOST_OPTIONAL

#endif // RVARAGO_ABSENT_BOOST_OPTIONAL_H
