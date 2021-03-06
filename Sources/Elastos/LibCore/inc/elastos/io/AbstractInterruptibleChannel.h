#ifndef __ELASTOS_IO_CHANNELS_ABSTRACTINTERRUPTIBLECHANNEL_H__
#define __ELASTOS_IO_CHANNELS_ABSTRACTINTERRUPTIBLECHANNEL_H__

#include <Elastos.CoreLibrary_server.h>
#include <elastos/Thread.h>
#include <elastos/Mutex.h>

using Elastos::Core::IRunnable;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

/**
 * {@code AbstractInterruptibleChannel} is the root class for interruptible
 * channels.
 * <p>
 * The basic usage pattern for an interruptible channel is to invoke
 * {@code begin()} before any I/O operation that potentially blocks
 * indefinitely, then {@code end(boolean)} after completing the operation. The
 * argument to the {@code end} method should indicate if the I/O operation has
 * actually completed so that any change may be visible to the invoker.
 */
class AbstractInterruptibleChannel
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    Boolean IsOpen();

    CARAPI IsOpen(
        /* [in] */ Boolean* isOpen);

    /**
     * Closes an open channel. If the channel is already closed then this method
     * has no effect, otherwise it closes the receiver via the
     * {@code implCloseChannel} method.
     * <p>
     * If an attempt is made to perform an operation on a closed channel then a
     * {@link java.nio.channels.ClosedChannelException} is thrown.
     * <p>
     * If multiple threads attempt to simultaneously close a channel, then only
     * one thread will run the closure code and the others will be blocked until
     * the first one completes.
     *
     * @throws IOException
     *             if a problem occurs while closing this channel.
     * @see java.nio.channels.Channel#close()
     */
    CARAPI Close();

    void SetInterrupted(Boolean value)
    {
        mInterrupted = value;
    }

protected:
    AbstractInterruptibleChannel();

    virtual ~AbstractInterruptibleChannel();

    /**
     * Implements the channel closing behavior.
     * <p>
     * Closes the channel with a guarantee that the channel is not currently
     * closed through another invocation of {@code close()} and that the method
     * is thread-safe.
     * <p>
     * Any outstanding threads blocked on I/O operations on this channel must be
     * released with either a normal return code, or by throwing an
     * {@code AsynchronousCloseException}.
     *
     * @throws IOException
     *             if a problem occurs while closing the channel.
     */
    virtual CARAPI ImplCloseChannel() = 0;

    /**
     * Indicates the beginning of a code section that includes an I/O operation
     * that is potentially blocking. After this operation, the application
     * should invoke the corresponding {@code end(boolean)} method.
     */
    void Begin();

    /**
     * Indicates the end of a code section that has been started with
     * {@code begin()} and that includes a potentially blocking I/O operation.
     *
     * @param success
     *            pass {@code true} if the blocking operation has succeeded and
     *            has had a noticeable effect; {@code false} otherwise.
     * @throws AsynchronousCloseException
     *             if this channel is closed by another thread while this method
     *             is executing.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while this
     *             method is executing.
     */
    CARAPI End(Boolean success);

private:
    volatile Boolean mClosed;
    volatile Boolean mInterrupted;
    AutoPtr<IRunnable> mInterruptAndCloseRunnable;
    Mutex mMutex;
};

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif

