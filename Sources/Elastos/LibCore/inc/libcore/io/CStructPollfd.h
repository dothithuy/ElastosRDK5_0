
#ifndef __CSTRUCTPOLLFD_H__
#define __CSTRUCTPOLLFD_H__

#include "_CStructPollfd.h"

namespace Libcore {
namespace IO {

/**
 * Corresponds to C's {@code struct pollfd} from
 * <a href="http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/poll.h.html">&lt;poll.h&gt;</a>
 */
CarClass(CStructPollfd)
{
public:
    /** The file descriptor to poll. */
    CARAPI GetFd(
        /* [out] */ Int32* fd);

    CARAPI SetFd(
        /* [in] */ Int32 fd);

    /**
     * The events we're interested in. POLLIN corresponds to being in select(2)'s read fd set,
     * POLLOUT to the write fd set.
     */
    CARAPI GetEvents(
        /* [out] */ Int16* events);

    CARAPI SetEvents(
        /* [in] */ Int16 events);

    /** The events that actually happened. */
    CARAPI GetRevents(
        /* [out] */ Int16* revents);

    CARAPI SetRevents(
        /* [in] */ Int16 revents);

    /**
     * A non-standard extension that lets callers conveniently map back to the object
     * their fd belongs to. This is used by Selector, for example, to associate each
     * FileDescriptor with the corresponding SelectionKey.
     */
    CARAPI GetUserData(
        /* [out] */ IInterface** userData);

    CARAPI SetUserData(
        /* [in] */ IInterface* userData);

private:
    /** The file descriptor to poll. */
    Int32 mFd;

    /**
     * The events we're interested in. POLLIN corresponds to being in select(2)'s read fd set,
     * POLLOUT to the write fd set.
     */
    Int16 mEvents;

    /** The events that actually happened. */
    Int16 mRevents;

    /**
     * A non-standard extension that lets callers conveniently map back to the object
     * their fd belongs to. This is used by Selector, for example, to associate each
     * FileDescriptor with the corresponding SelectionKey.
     */
    AutoPtr<IInterface> mUserData;
};

} // namespace IO
} // namespace Libcore

#endif //__CSTRUCTPOLLFD_H__
