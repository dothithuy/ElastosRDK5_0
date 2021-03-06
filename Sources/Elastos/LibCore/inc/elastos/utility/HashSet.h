#ifndef __UTILITY_HASHSET_H__
#define __UTILITY_HASHSET_H__

#include "AbstractSet.h"

using Elastos::IO::IObjectInputStream;
using Elastos::IO::IObjectOutputStream;

namespace Elastos {
namespace Utility {

class HashSet : public AbstractSet
{
public:
    /**
     * Adds the specified object to this {@code HashSet} if not already present.
     *
     * @param object
     *            the object to add.
     * @return {@code true} when this {@code HashSet} did not already contain
     *         the object, {@code false} otherwise
     */
    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    /**
     * Removes all elements from this {@code Collection}, leaving it empty (optional).
     *
     * @throws UnsupportedOperationException
     *                if removing from this {@code Collection} is not supported.
     *
     * @see #isEmpty
     * @see #size
     */
    CARAPI Clear();

    /**
     * Returns a new {@code HashSet} with the same elements and size as this
     * {@code HashSet}.
     *
     * @return a shallow copy of this {@code HashSet}.
     * @see java.lang.Cloneable
     */
    // @Override
    // @SuppressWarnings("unchecked")
    CARAPI Clone(
        /* [out] */ IInterface** outface);

    /**
     * Searches this {@code HashSet} for the specified object.
     *
     * @param object
     *            the object to search for.
     * @return {@code true} if {@code object} is an element of this
     *         {@code HashSet}, {@code false} otherwise.
     */
    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    /**
     * Returns true if this {@code HashSet} has no elements, false otherwise.
     *
     * @return {@code true} if this {@code HashSet} has no elements,
     *         {@code false} otherwise.
     * @see #size
     */
    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    /**
     * Removes the specified object from this {@code HashSet}.
     *
     * @param object
     *            the object to remove.
     * @return {@code true} if the object was removed, {@code false} otherwise.
     */
    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    /**
     * Returns the number of elements in this {@code HashSet}.
     *
     * @return the number of elements in this {@code HashSet}.
     */
    CARAPI GetSize(
        /* [out] */ Int32* size);

    /**
     * Returns an Iterator on the elements of this {@code HashSet}.
     *
     * @return an Iterator on the elements of this {@code HashSet}.
     * @see Iterator
     */
    CARAPI GetIterator(
        /* [out] */ IIterator** it);

protected:
    HashSet() {}

    /**
     * Constructs a new empty instance of {@code HashSet}.
     */
    CARAPI Init();

    /**
     * Constructs a new instance of {@code HashSet} with the specified capacity.
     *
     * @param capacity
     *            the initial capacity of this {@code HashSet}.
     */
    CARAPI Init(
        /* [in] */ Int32 capacity);

    /**
     * Constructs a new instance of {@code HashSet} with the specified capacity
     * and load factor.
     *
     * @param capacity
     *            the initial capacity.
     * @param loadFactor
     *            the initial load factor.
     */
    CARAPI Init(
        /* [in] */ Int32 capacity,
        /* [in] */ Float loadFactor);

    /**
     * Constructs a new instance of {@code HashSet} containing the unique
     * elements in the specified collection.
     *
     * @param collection
     *            the collection of elements to add.
     */
    CARAPI Init(
        /* [in] */ ICollection* collection);

    CARAPI Init(
        /* [in] */ IMap* backingMap);

    virtual CARAPI_(AutoPtr<IMap>) CreateBackingMap(
        /* [in] */ Int32 capacity,
        /* [in] */ Float loadFactor);

private:
    CARAPI WriteObject(
        /* [in] */ IObjectOutputStream* stream);

    // @SuppressWarnings("unchecked")
    CARAPI ReadObject(
        /* [in] */ IObjectInputStream* stream);

private:
    static const Int64 sSerialVersionUID = -5024744406713321676L;

    /* transient */ AutoPtr<IMap> mBackingMap;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_HASHSET_H__