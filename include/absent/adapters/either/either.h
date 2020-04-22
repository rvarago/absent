#ifndef RVARAGO_ABSENT_ADAPTERS_EITHER_H
#define RVARAGO_ABSENT_ADAPTERS_EITHER_H

#include <variant>

namespace rvarago::absent::adapters::types {
template <typename A, typename E>
using either = std::variant<A, E>;
}

#endif
