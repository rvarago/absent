#ifndef RVARAGO_ABSENT_SUPPORT_EXECUTIONSTATUS_H
#define RVARAGO_ABSENT_SUPPORT_EXECUTIONSTATUS_H

#include <optional>

#include "absent/support/sink.h"

namespace rvarago::absent::support {

/**
 * Defines type that holds only one value, which is blank{}, also known as unit.
 */
struct blank final {};

inline constexpr auto unit = blank{};

constexpr bool operator==(blank, blank) noexcept {
    return true;
}

constexpr bool operator!=(blank, blank) noexcept {
    return false;
}

constexpr bool operator<(blank, blank) noexcept {
    return false;
}

constexpr bool operator>(blank, blank) noexcept {
    return false;
}

constexpr bool operator<=(blank, blank) noexcept {
    return true;
}

constexpr bool operator>=(blank, blank) noexcept {
    return true;
}

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

namespace std {

template <>
struct hash<rvarago::absent::support::blank> {
    constexpr std::size_t operator()(rvarago::absent::support::blank const &) const noexcept {
        constexpr auto magic_blank_hash = static_cast<std::size_t>(-7777);
        return magic_blank_hash;
    }
};

}

#endif
