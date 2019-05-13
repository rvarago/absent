#ifndef RVARAGO_ABSENT_DETAILS_H
#define RVARAGO_ABSENT_DETAILS_H

namespace rvarago::absent::details {
    template<typename A, typename B>
    using MapperMember = B (A::*)() const;
}

#endif //RVARAGO_ABSENT_DETAILS_H
