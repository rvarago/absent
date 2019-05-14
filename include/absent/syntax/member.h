#ifndef RVARAGO_ABSENT_MEMBER_H
#define RVARAGO_ABSENT_MEMBER_H

namespace rvarago::absent::syntax::member {
    template<typename A, typename B>
    using Mapper = B (A::*)() const;
}

#endif //RVARAGO_ABSENT_MEMBER_H
