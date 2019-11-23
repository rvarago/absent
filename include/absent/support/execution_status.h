#ifndef RVARAGO_ABSENT_EXECUTIONSTATUS_H
#define RVARAGO_ABSENT_EXECUTIONSTATUS_H

#include "absent/support/blank.h"
#include <optional>

namespace rvarago::absent::support {
/**
 * Type for a computation that may or may not succeed.
 */
using execution_status = std::optional<blank>;

/**
 * For a successful computation.
 */
inline constexpr execution_status success = std::make_optional(unit);

/**
 * For a failed computation.
 */
inline constexpr execution_status failure = std::nullopt;
}

#endif
