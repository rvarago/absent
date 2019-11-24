#ifndef RVARAGO_ABSENT_SINK_H
#define RVARAGO_ABSENT_SINK_H

#include <utility>

namespace rvarago::absent::support {

/**
 * Wraps a callable that should receive parameters in another callable that discards the referred parameters.
 *
 * @param f callable to be wrapped.
 * @return a new callable that discards the parameters sent to it.
 */
template <typename NullaryFunction>
constexpr decltype(auto) sink(NullaryFunction &&f) noexcept {
    return [f = std::forward<NullaryFunction>(f)](auto &&...) { return f(); };
}

}

#endif