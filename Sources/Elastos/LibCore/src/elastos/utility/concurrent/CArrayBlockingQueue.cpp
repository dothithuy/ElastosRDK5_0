
#include "cmdef.h"
#include "CArrayBlockingQueue.h"
#include "CReentrantLock.h"
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>
#include <elastos/Math.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::Concurrent::Locks::CReentrantLock;

namespace Elastos {
namespace Utility {
namespace Concurrent {


//==========================================================
//       CTreeMap::CArrayBlockingQueue
//==========================================================
CArrayBlockingQueue::CArrayBlockingQueue()
    : mTakeIndex(0)
    , mPutIndex(0)
    , mCount(0)
{
}

PInterface CArrayBlockingQueue::Probe(
    /* [in] */ REIID riid)
{
    return _CArrayBlockingQueue::Probe(riid);
}

ECode CArrayBlockingQueue::constructor(
    /* [in] */ Int32 capacity)
{
    return this->constructor(capacity, FALSE);
}

ECode CArrayBlockingQueue::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Boolean fair)
{
    if (capacity <= 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mItems = ArrayOf<IInterface*>::Alloc(capacity);
    CReentrantLock::New(fair, (IReentrantLock**)&mLock);
    mLock->NewCondition((ICondition**)&mNotEmpty);
    mLock->NewCondition((ICondition**)&mNotFull);
    return NOERROR;
}

ECode CArrayBlockingQueue::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Boolean fair,
    /* [in] */ ICollection* c)
{
    FAIL_RETURN(this->constructor(capacity, fair));

    AutoPtr<IReentrantLock> lock = mLock;
    ECode ec = NOERROR;
    Int32 i = 0;
    AutoPtr<IIterator> it;
    Boolean hasNext;
    AutoPtr<IInterface> obj;

    lock->Lock(); // Lock only for visibility, not mutual exclusion

    // try {
    i = 0;
        // try {
    if (c != NULL) {
        c->GetIterator((IIterator**)&it);
        while(it->HasNext(&hasNext), hasNext) {
            obj = NULL;
            it->Next((IInterface**)&obj);
            ec = CheckNotNull(obj);
            if (FAILED(ec)) goto _Exit_;

            if (i < capacity)
                mItems->Set(i++, obj);
            else {
                ec = E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
                goto _Exit_;
            }
        }
    }
    // } catch (ArrayIndexOutOfBoundsException ex) {
        // throw new IllegalArgumentException();
    // }
    mCount = i;
    mPutIndex = (i == capacity) ? 0 : i;
    // } finally {
_Exit_:
    lock->UnLock();
    // }

    return ec;
}

ECode CArrayBlockingQueue::Add(
    /* [in] */ IInterface* e,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::Add(e, modified);
}

ECode CArrayBlockingQueue::Offer(
    /* [in] */ IInterface* e,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    FAIL_RETURN(CheckNotNull(e));

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    if (mCount != mItems->GetLength()) {
        Enqueue(e);
        *result = TRUE;
    }
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::Put(
    /* [in] */ IInterface* e)
{
    FAIL_RETURN(CheckNotNull(e));

    AutoPtr<IReentrantLock> lock = mLock;
    lock->LockInterruptibly();
    // try {
    while (mCount == mItems->GetLength()) {
        mNotFull->Await();
    }
    Enqueue(e);
    // } finally {
         lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::OfferEx(
    /* [in] */ IInterface* e,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    FAIL_RETURN(CheckNotNull(e));
    VALIDATE_NOT_NULL(unit);

    Int64 nanos;
    unit->ToNanos(timeout, &nanos);

    AutoPtr<IReentrantLock> lock = mLock;
    lock->LockInterruptibly();

    // try {
    while (mCount == mItems->GetLength()) {
        if (nanos <= 0) {
            goto _Exit_;
        }
        mNotFull->AwaitNanos(nanos, &nanos);
    }

    Enqueue(e);
    *result = TRUE;

_Exit_:
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::Poll(
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    if (mCount != 0) {
        AutoPtr<IInterface> obj = Dequeue();
        *e = obj;
        INTERFACE_ADDREF(*e);
    }
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::Take(
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;

    AutoPtr<IReentrantLock> lock = mLock;
    lock->LockInterruptibly();
    // try {
    while (mCount == 0) {
        mNotEmpty->Await();
    }
    AutoPtr<IInterface> obj = Dequeue();
    *e = obj;
    INTERFACE_ADDREF(*e);
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::PollEx(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;

    Int64 nanos;
    unit->ToNanos(timeout, &nanos);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->LockInterruptibly();
    // try {
    while (mCount == 0) {
        if (nanos <= 0) {
            lock->UnLock();
            return NOERROR;
        }

        mNotEmpty->AwaitNanos(nanos, &nanos);
    }

    AutoPtr<IInterface> obj = Dequeue();
    *e = obj;
    INTERFACE_ADDREF(*e);
    lock->UnLock();
    // } finally {
    //     lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::Peek(
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    if (mCount != 0) {
        AutoPtr<IInterface> obj = ItemAt(mTakeIndex);
        *e = obj;
        INTERFACE_ADDREF(*e);
    }
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::Element(
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;
    return E_NOT_IMPLEMENTED;
}

ECode CArrayBlockingQueue::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    *size = mCount;
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::RemainingCapacity(
    /* [out] */ Int32* capacity)
{
    VALIDATE_NOT_NULL(capacity);

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    *capacity = mItems->GetLength() - mCount;
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractQueue::ContainsAll(collection, result);
}

ECode CArrayBlockingQueue::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = object == this->Probe(EIID_IInterface);
    return NOERROR;
}

ECode CArrayBlockingQueue::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

ECode CArrayBlockingQueue::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractQueue::IsEmpty(result);
}

ECode CArrayBlockingQueue::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    if (o == NULL) return NOERROR;

    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    if (mCount > 0) {
        Int32 putIndex = mPutIndex;
        Int32 i = mTakeIndex;
        Boolean equals;
        do {
            assert(0 && "TODO");
            // if (o->Equals((*items)[i], &equals), equals) {
            //     RemoveAt(i);
            //     *result = TRUE;
            //     lock->UnLock();
            //     return NOERROR;
            // }
        } while ((i = Inc(i)) != putIndex);
    }
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::RemoveEx(
    /* [out] */ IInterface** e)
{
    return AbstractQueue::RemoveEx(e);
}

ECode CArrayBlockingQueue::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::RemoveAll(collection, modified);
}

ECode CArrayBlockingQueue::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::RetainAll(collection, modified);
}

ECode CArrayBlockingQueue::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    if (o == NULL) return NOERROR;

    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    if (mCount > 0) {
        Int32 putIndex = mPutIndex;
        Int32 i = mTakeIndex;
        Boolean equals;
        do {
            assert(0 && "TODO");
            // if (o->Equals((*items)[i], &equals), equals) {
            //     *result = TRUE;
            //     lock->UnLock();
            //     return NOERROR;
            // }
        } while ((i = Inc(i)) != putIndex);
    }
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);

    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    Int32 count = mCount;
    AutoPtr<ArrayOf<IInterface*> > a = ArrayOf<IInterface*>::Alloc(count);
    for (Int32 i = mTakeIndex, k = 0; k < count; i = Inc(i), k++){
        a->Set(k, (*items)[i]);
    }
    *array = a;
    INTERFACE_ADDREF(*array);
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(a);

    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    Int32 count = mCount;
    AutoPtr<ArrayOf<IInterface*> > r = a;
    Int32 len = r ? r->GetLength() : 0;
    if (len < count) {
        r = ArrayOf<IInterface*>::Alloc(count);
    }

    for (Int32 i = mTakeIndex, k = 0; k < count; i = Inc(i), k++) {
        a->Set(k, (*items)[i]);
    }

    if (len > count) {
        for (Int32 i = count; i < len; ++i) {
            a->Set(i, NULL);
        }
    }

    *outArray = r;
    INTERFACE_ADDREF(*outArray);
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    Int32 k = mCount;
    if (k == 0) {
        *str = String("[]");
        lock->UnLock();
        return NOERROR;
    }

    StringBuilder sb;
    sb += "[";
    for (Int32 i = mTakeIndex; ; i = Inc(i)) {
        AutoPtr<IInterface> e = (*mItems)[i];
        if (e.Get() == this->Probe(EIID_IInterface)) {
            sb += "(this Collection)";
        }
        else {
            sb += StringUtils::Int32ToHexString((Int32)e.Get());
        }
        if (--k == 0) {
            sb += "]";
            goto _Exit_;
        }

        sb += ", ";
    }

_Exit_:
    *str = sb.ToString();
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_NOT_IMPLEMENTED;
}

ECode CArrayBlockingQueue::Clear()
{
    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    Int32 k = mCount;
    if (k > 0) {
        Int32 putIndex = mPutIndex;
        Int32 i = mTakeIndex;
        do {
            mItems->Set(i, NULL);
        } while ((i = Inc(i)) != putIndex);

        mTakeIndex = putIndex;
        mCount = 0;
        if (mItrs != NULL)
            mItrs->QueueIsEmpty();

        Boolean hasWaiters;
        for (; k > 0 && (lock->HasWaiters(mNotFull, &hasWaiters), hasWaiters); k--)
            mNotFull->Signal();
    }
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::DrainTo(
    /* [in] */ ICollection* c,
    /* [out] */ Int32* number)
{
    return DrainToEx(c, Elastos::Core::Math::INT32_MAX_VALUE, number);
}

ECode CArrayBlockingQueue::DrainToEx(
    /* [in] */ ICollection* c,
    /* [in] */ Int32 maxElements,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = 0;

    FAIL_RETURN(CheckNotNull(c));

    if (c == THIS_PROBE(ICollection)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (maxElements <= 0) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    // try {
    Int32 n = Elastos::Core::Math::Min(maxElements, mCount);
    Int32 take = mTakeIndex;
    Int32 i = 0;
    Boolean bval;
    // try {
    while (i < n) {
        AutoPtr<IInterface> x = (*items)[take];
        c->Add(x, &bval);
        items->Set(take, NULL);
        take = Inc(take);
        i++;
    }
    *number = n;
    // } finally {
    // Restore invariants even if c.add() threw
    if (i > 0) {
        mCount -= i;
        mTakeIndex = take;
        if (mItrs != NULL) {
            if (mCount == 0)
                mItrs->QueueIsEmpty();
            else if (i > take)
                mItrs->TakeIndexWrapped();
        }

        Boolean hasWaiters;
        for (; i > 0 && (lock->HasWaiters(mNotFull, &hasWaiters), hasWaiters); i--) {
            mNotFull->Signal();
        }
    }
    // }
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<IIterator> iter = new Itr(this);
    *it = iter;
    INTERFACE_ADDREF(*it);
    return NOERROR;
}

Int32 CArrayBlockingQueue::Inc(
    /* [in] */ Int32 i)
{
    return (++i == mItems->GetLength()) ? 0 : i;
}

Int32 CArrayBlockingQueue::Dec(
    /* [in] */ Int32 i)
{
    return ((i == 0) ? mItems->GetLength() : i) - 1;
}

AutoPtr<IInterface> CArrayBlockingQueue::ItemAt(
    /* [in] */ Int32 i)
{
    AutoPtr<IInterface> obj;
    if (i < mItems->GetLength()) {
        obj = (*mItems)[i];
    }
    return obj;
}

ECode CArrayBlockingQueue::CheckNotNull(
    /* [in] */ IInterface* v)
{
    VALIDATE_NOT_NULL(v);
    return NOERROR;
}

ECode CArrayBlockingQueue::Enqueue(
    /* [in] */ IInterface* x)
{
    // assert lock.getHoldCount() == 1;
    // assert items[putIndex] == NULL;
    mItems->Set(mPutIndex, x);
    mPutIndex = Inc(mPutIndex);
    mCount++;
    mNotEmpty->Signal();
    return NOERROR;
}

AutoPtr<IInterface> CArrayBlockingQueue::Dequeue()
{
    // assert lock.getHoldCount() == 1;
    // assert items[takeIndex] != NULL;
    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    AutoPtr<IInterface> x = (*items)[mTakeIndex];
    items->Set(mTakeIndex, NULL);
    mTakeIndex = Inc(mTakeIndex);
    mCount--;
    if (mItrs != NULL)
        mItrs->ElementDequeued();
    mNotFull->Signal();
    return x;
}

ECode CArrayBlockingQueue::RemoveAt(
    /* [in] */ Int32 removeIndex)
{
    // assert lock.getHoldCount() == 1;
    // assert items[removeIndex] != NULL;
    // assert removeIndex >= 0 && removeIndex < mItems->GetLength();
    AutoPtr<ArrayOf<IInterface*> > items = mItems;
    if (removeIndex == mTakeIndex) {
        // removing front item; just advance
        items->Set(mTakeIndex, NULL);
        mTakeIndex = Inc(mTakeIndex);
        mCount--;
        if (mItrs != NULL)
            mItrs->ElementDequeued();
    }
    else {
        // an "interior" remove

        // slide over all others up through putIndex.
        Int32 putIndex = mPutIndex;
        for (Int32 i = removeIndex;;) {
            Int32 next = Inc(i);
            if (next != putIndex) {
                items->Set(i, (*items)[next]);
                i = next;
            }
            else {
                items->Set(i, NULL);
                mPutIndex = i;
                break;
            }
        }
        mCount--;
        if (mItrs != NULL)
            mItrs->RemovedAt(removeIndex);
    }
    mNotFull->Signal();
    return E_NOT_IMPLEMENTED;
}


//==========================================================
//       CTreeMap::Itrs
//==========================================================
const Int32 CArrayBlockingQueue::Itrs::SHORT_SWEEP_PROBES = 4;
const Int32 CArrayBlockingQueue::Itrs::LONG_SWEEP_PROBES = 16;

CArrayBlockingQueue::Itrs::Itrs(
    /* [in] */ Itr* initial)
{
    Register(initial);
    mCycles = 0;
}

ECode CArrayBlockingQueue::Itrs::DoSomeSweeping(
    /* [in] */ Boolean tryHarder)
{
    // assert lock.getHoldCount() == 1;
    // assert head != NULL;
    // Int32 probes = tryHarder ? LONG_SWEEP_PROBES : SHORT_SWEEP_PROBES;
    // AutoPtr<Node> o, p;
    // AutoPtr<Node> sweeper = mSweeper;
    // Boolean passedGo;   // to limit search to one full sweep

    // if (sweeper == NULL) {
    //     o = NULL;
    //     p = head;
    //     passedGo = TRUE;
    // }
    // else {
    //     o = sweeper;
    //     p = o->mNext;
    //     passedGo = FALSE;
    // }

    // for (; probes > 0; probes--) {
    //     if (p == NULL) {
    //         if (passedGo)
    //             break;
    //         o = NULL;
    //         p = head;
    //         passedGo = TRUE;
    //     }
    //     Itr it = p.get();
    //     Node next = p.next;
    //     if (it == NULL || it.IsDetached()) {
    //         // found a discarded/exhausted iterator
    //         probes = LONG_SWEEP_PROBES; // "try harder"
    //         // unlink p
    //         p.clear();
    //         p.next = NULL;
    //         if (o == NULL) {
    //             head = next;
    //             if (next == NULL) {
    //                 // We've run out of iterators to track; retire
    //                 itrs = NULL;
    //                 return;
    //             }
    //         }
    //         else
    //             o.next = next;
    //     } else {
    //         o = p;
    //     }
    //     p = next;
    // }

    // this.sweeper = (p == NULL) ? NULL : o;
    return E_NOT_IMPLEMENTED;
}

ECode CArrayBlockingQueue::Itrs::Register(
    /* [in] */ Itr* itr)
{
    // assert lock.getHoldCount() == 1;
    // mHead = new Node(itr, head);
    return E_NOT_IMPLEMENTED;
}

ECode CArrayBlockingQueue::Itrs::TakeIndexWrapped()
{
    // // assert lock.getHoldCount() == 1;
    // cycles++;
    // for (Node o = NULL, p = head; p != NULL;) {
    //     Itr it = p.get();
    //     Node next = p.next;
    //     if (it == NULL || it.takeIndexWrapped()) {
    //         // unlink p
    //         // assert it == NULL || it.IsDetached();
    //         p.clear();
    //         p.next = NULL;
    //         if (o == NULL)
    //             head = next;
    //         else
    //             o.next = next;
    //     } else {
    //         o = p;
    //     }
    //     p = next;
    // }
    // if (head == NULL)   // no more iterators to track
    //     itrs = NULL;
    return E_NOT_IMPLEMENTED;
}

ECode CArrayBlockingQueue::Itrs::RemovedAt(
    /* [in] */ Int32 removedIndex)
{
    // for (Node o = NULL, p = head; p != NULL;) {
    //     Itr it = p.get();
    //     Node next = p.next;
    //     if (it == NULL || it.removedAt(removedIndex)) {
    //         // unlink p
    //         // assert it == NULL || it.IsDetached();
    //         p.clear();
    //         p.next = NULL;
    //         if (o == NULL)
    //             head = next;
    //         else
    //             o.next = next;
    //     } else {
    //         o = p;
    //     }
    //     p = next;
    // }
    // if (head == NULL)   // no more iterators to track
    //     itrs = NULL;
    return E_NOT_IMPLEMENTED;
}

ECode CArrayBlockingQueue::Itrs::QueueIsEmpty()
{
    // // assert lock.getHoldCount() == 1;
    // for (Node p = head; p != NULL; p = p.next) {
    //     Itr it = p.get();
    //     if (it != NULL) {
    //         p.clear();
    //         it.shutdown();
    //     }
    // }
    // head = NULL;
    // itrs = NULL;
    return E_NOT_IMPLEMENTED;
}

ECode CArrayBlockingQueue::Itrs::ElementDequeued()
{
    // assert lock.getHoldCount() == 1;
    // if (mCount == 0)
    //     QueueIsEmpty();
    // else if (mTakeIndex == 0)
    //     TakeIndexWrapped();
    return NOERROR;
}


//==========================================================
//       CTreeMap::Itr
//==========================================================
const Int32 CArrayBlockingQueue::Itr::NONE = -1;
const Int32 CArrayBlockingQueue::Itr::REMOVED = -2;
const Int32 CArrayBlockingQueue::Itr::DETACHED = -3;

CAR_INTERFACE_IMPL(CArrayBlockingQueue::Itr, IIterator);

CArrayBlockingQueue::Itr::Itr(
    /* [in] */ CArrayBlockingQueue* host)
    : mHost(host)
    , mCursor(0)
    , mNextIndex(0)
    , mPrevTakeIndex(0)
    , mPrevCycles(0)
{
    // assert lock.getHoldCount() == 0;
    mLastRet = NONE;
    AutoPtr<IReentrantLock> lock = mHost->mLock;
    lock->Lock();
    // try {
        if (mHost->mCount == 0) {
            // assert itrs == NULL;
            mCursor = NONE;
            mNextIndex = NONE;
            mPrevTakeIndex = DETACHED;
        }
        else {
            Int32 takeIndex = mHost->mTakeIndex;
            mPrevTakeIndex = takeIndex;
            mNextItem = mHost->ItemAt(mNextIndex = takeIndex);
            mCursor = IncCursor(takeIndex);
            if (mHost->mItrs == NULL) {
                mHost->mItrs = new Itrs(this);
            }
            else {
                mHost->mItrs->Register(this); // in this order
                mHost->mItrs->DoSomeSweeping(FALSE);
            }
            mPrevCycles = mHost->mItrs->mCycles;
            // assert takeIndex >= 0;
            // assert prevTakeIndex == takeIndex;
            // assert nextIndex >= 0;
            // assert nextItem != NULL;
        }
    // } finally {
        lock->UnLock();
    // }
}

Boolean CArrayBlockingQueue::Itr::IsDetached()
{
    // // assert lock.getHoldCount() == 1;
    return mPrevTakeIndex < 0;
}

ECode CArrayBlockingQueue::Itr::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // assert lock.getHoldCount() == 0;
    if (mNextItem != NULL) {
        *result = TRUE;
        return NONE;
    }
    NoNext();
    *result = FALSE;
    return NOERROR;
}

ECode CArrayBlockingQueue::Itr::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = NULL;

    // assert lock.getHoldCount() == 0;
    AutoPtr<IInterface> x = mNextItem;
    if (x == NULL) {
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }

    AutoPtr<IReentrantLock> lock = mHost->mLock;
    lock->Lock();
    // try {
    if (!IsDetached()) {
        IncorporateDequeues();
    }
    // assert nextIndex != NONE;
    // assert lastItem == NULL;
    mLastRet = mNextIndex;
    Int32 cursor = mCursor;
    if (cursor >= 0) {
        mNextItem = mHost->ItemAt(mNextIndex = cursor);
        // assert nextItem != NULL;
        mCursor = IncCursor(cursor);
    }
    else {
        mNextIndex = NONE;
        mNextItem = NULL;
    }
    // } finally {
    lock->UnLock();
    // }
    *object = x;
    INTERFACE_ADDREF(*object);
    return NOERROR;
}

ECode CArrayBlockingQueue::Itr::Remove()
{
    // assert lock.getHoldCount() == 0;
    AutoPtr<IReentrantLock> lock = mHost->mLock;
    lock->Lock();
    // try {
    if (!IsDetached()) {
        IncorporateDequeues(); // might update mLastRet or detach
    }

    Int32 lastRet = mLastRet;
    mLastRet = NONE;
    if (lastRet >= 0) {
        if (!IsDetached()) {
            mHost->RemoveAt(lastRet);
        }
        else {
            AutoPtr<IInterface> lastItem = mLastItem;
            // assert lastItem != NULL;
            mLastItem = NULL;
            AutoPtr<IInterface> item = mHost->ItemAt(lastRet);
            if (item == lastItem) {
                mHost->RemoveAt(lastRet);
            }
        }
    }
    else if (lastRet == NONE) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // else mLastRet == REMOVED and the last returned element was
    // previously asynchronously removed via an operation other
    // than this.remove(), so nothing to do.

    if (mCursor < 0 && mNextIndex < 0)
        Detach();
    // } finally {
    lock->UnLock();
    // assert mLastRet == NONE;
    // assert lastItem == NULL;
    // }
    return NOERROR;
}

ECode CArrayBlockingQueue::Itr::Shutdown()
{
    // assert lock.getHoldCount() == 1;
    mCursor = NONE;
    if (mNextIndex >= 0)
        mNextIndex = REMOVED;
    if (mLastRet >= 0) {
        mLastRet = REMOVED;
        mLastItem = NULL;
    }
    mPrevTakeIndex = DETACHED;
    // Don't set nextItem to NULL because we must continue to be
    // able to return it on next().
    //
    // Caller will unlink from itrs when convenient.
    return E_NOT_IMPLEMENTED;
}

Boolean CArrayBlockingQueue::Itr::RemovedAt(
    /* [in] */ Int32 removedIndex)
{
    // assert lock.getHoldCount() == 1;
    if (IsDetached())
        return TRUE;

    Int32 cycles = mHost->mItrs->mCycles;
    Int32 takeIndex = mHost->mTakeIndex;
    Int32 prevCycles = mPrevCycles;
    Int32 prevTakeIndex = mPrevTakeIndex;
    Int32 len = mHost->mItems->GetLength();
    Int32 cycleDiff = cycles - prevCycles;
    if (removedIndex < takeIndex) {
        cycleDiff++;
    }
    Int32 removedDistance = (cycleDiff * len) + (removedIndex - prevTakeIndex);

    // assert removedDistance >= 0;
    Int32 cursor = mCursor;
    if (cursor >= 0) {
        Int32 x = Distance(cursor, prevTakeIndex, len);
        if (x == removedDistance) {
            if (cursor == mHost->mPutIndex)
                mCursor = cursor = NONE;
        }
        else if (x > removedDistance) {
            // assert cursor != prevTakeIndex;
            mCursor = cursor = mHost->Dec(cursor);
        }
    }
    Int32 lastRet = mLastRet;
    if (lastRet >= 0) {
        Int32 x = Distance(lastRet, prevTakeIndex, len);
        if (x == removedDistance)
            mLastRet = lastRet = REMOVED;
        else if (x > removedDistance)
            mLastRet = lastRet = mHost->Dec(lastRet);
    }

    Int32 nextIndex = mNextIndex;
    if (nextIndex >= 0) {
        Int32 x = Distance(nextIndex, prevTakeIndex, len);
        if (x == removedDistance)
            mNextIndex = nextIndex = REMOVED;
        else if (x > removedDistance)
            mNextIndex = nextIndex = mHost->Dec(nextIndex);
    }
    else if (cursor < 0 && nextIndex < 0 && lastRet < 0) {
        mPrevTakeIndex = DETACHED;
        return TRUE;
    }

    return FALSE;
}

Boolean CArrayBlockingQueue::Itr::TakeIndexWrapped()
{
    // assert lock.getHoldCount() == 1;
    if (IsDetached())
        return TRUE;

    if (mHost->mItrs->mCycles - mPrevCycles > 1) {
        // All the elements that existed at the time of the last
        // operation are gone, so abandon further iteration.
        Shutdown();
        return TRUE;
    }
    return FALSE;
}

Int32 CArrayBlockingQueue::Itr::IncCursor(
    /* [in] */ Int32 index)
{
    // assert lock.getHoldCount() == 1;
    index = mHost->Inc(index);
    if (index == mHost->mPutIndex)
        index = NONE;
    return index;
}

Boolean CArrayBlockingQueue::Itr::Invalidated(
    /* [in] */ Int32 index,
    /* [in] */ Int32 prevTakeIndex,
    /* [in] */ Int64 dequeues,
    /* [in] */ Int32 length)
{
    if (index < 0)
        return FALSE;
    Int32 distance = index - prevTakeIndex;
    if (distance < 0)
        distance += length;
    return dequeues > distance;
}

ECode CArrayBlockingQueue::Itr::IncorporateDequeues()
{
    // assert lock.getHoldCount() == 1;
    // assert itrs != NULL;
    // assert !IsDetached();
    // assert mCount > 0;

    Int32 cycles = mHost->mItrs->mCycles;
    Int32 takeIndex = mHost->mTakeIndex;
    Int32 prevCycles = mPrevCycles;
    Int32 prevTakeIndex = mPrevTakeIndex;

    if (cycles != prevCycles || takeIndex != prevTakeIndex) {
        Int32 len = mHost->mItems->GetLength();
        // how far takeIndex has advanced since the previous
        // operation of this iterator
        Int64 dequeues = (cycles - prevCycles) * len
            + (takeIndex - prevTakeIndex);

        // Check indices for invalidation
        if (Invalidated(mLastRet, prevTakeIndex, dequeues, len))
            mLastRet = REMOVED;
        if (Invalidated(mNextIndex, prevTakeIndex, dequeues, len))
            mNextIndex = REMOVED;
        if (Invalidated(mCursor, prevTakeIndex, dequeues, len))
            mCursor = takeIndex;

        if (mCursor < 0 && mNextIndex < 0 && mLastRet < 0) {
            Detach();
        }
        else {
            mPrevCycles = cycles;
            mPrevTakeIndex = takeIndex;
        }
    }
    return NOERROR;
}

ECode CArrayBlockingQueue::Itr::Detach()
{
    // Switch to detached mode
    // assert lock.getHoldCount() == 1;
    // assert cursor == NONE;
    // assert nextIndex < 0;
    // assert mLastRet < 0 || nextItem == NULL;
    // assert mLastRet < 0 ^ lastItem != NULL;
    if (mPrevTakeIndex >= 0) {
        // assert itrs != NULL;
        mPrevTakeIndex = DETACHED;
        // try to unlink from itrs (but not too hard)
        mHost->mItrs->DoSomeSweeping(TRUE);
    }
    return E_NOT_IMPLEMENTED;
}

Int32 CArrayBlockingQueue::Itr::Distance(
    /* [in] */ Int32 index,
    /* [in] */ Int32 prevTakeIndex,
    /* [in] */ Int32 length)
{
    Int32 distance = index - prevTakeIndex;
    if (distance < 0)
        distance += length;
    return distance;
}

ECode CArrayBlockingQueue::Itr::NoNext()
{
    AutoPtr<IReentrantLock> lock = mHost->mLock;
    lock->Lock();
    // try {
    // assert cursor == NONE;
    // assert nextIndex == NONE;
    if (!IsDetached()) {
        // assert mLastRet >= 0;
        IncorporateDequeues(); // might update mLastRet
        if (mLastRet >= 0) {
            mLastItem = mHost->ItemAt(mLastRet);
            // assert lastItem != NULL;
            Detach();
        }
    }
    // assert IsDetached();
    // assert mLastRet < 0 ^ lastItem != NULL;
    // } finally {
    lock->UnLock();
    // }
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
