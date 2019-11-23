#ifndef RVARAGO_ABSENT_MEMBER_H
#define RVARAGO_ABSENT_MEMBER_H

namespace rvarago::absent::support {
template <typename A, typename B>
using member_mapper = B (A::*)() const;
}

#endif // RVARAGO_ABSENT_MEMBER_H
