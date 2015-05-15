
#ifndef __ELASTOS_DROID_NET_CCREDENTIALS_H__
#define __ELASTOS_DROID_NET_CCREDENTIALS_H__

#include "_CCredentials.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A class for representing UNIX credentials passed via ancillary data
 * on UNIX domain sockets. See "man 7 unix" on a desktop linux distro.
 */
CarClass(CCredentials)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI GetPid(
        /* [out] */ Int32* pid);

    CARAPI GetUid(
        /* [out] */ Int32* uid);

    CARAPI GetGid(
        /* [out] */ Int32* gid);

private:
    /** pid of process. root peers may lie. */
    Int32 mPid;
    /** uid of process. root peers may lie. */
    Int32 mUid;
    /** gid of process. root peers may lie. */
    Int32 mGid;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos
#endif //__ELASTOS_DROID_NET_CCREDENTIALS_H__