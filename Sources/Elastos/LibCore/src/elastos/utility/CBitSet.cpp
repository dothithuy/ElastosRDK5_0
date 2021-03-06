
#include "cmdef.h"
#include "CBitSet.h"
#include "CByteBufferHelper.h"
#include <elastos/Math.h>
#include <elastos/Logger.h>
#include <elastos/StringBuilder.h>

using Elastos::Utility::Logging::Logger;

using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IByteBufferHelper;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Utility {

const Int64 CBitSet::ALL_ONES;

CBitSet::CBitSet()
    : mInt64Count(0)
{}

/**
 * Creates a new {@code BitSet} with size equal to 64 bits.
 */
ECode CBitSet::constructor()
{
    AutoPtr< ArrayOf<Int64> > arr = ArrayOf<Int64>::Alloc(1);
    return constructor(arr);
}

/**
 * Creates a new {@code BitSet} with size equal to {@code bitCount}, rounded up to
 * a multiple of 64.
 *
 * @throws NegativeArraySizeException if {@code bitCount < 0}.
 */
ECode CBitSet::constructor(
    /* [in] */ Int32 bitCount)
{
    if (bitCount < 0) {
        // throw new NegativeArraySizeException(Integer.toString(bitCount));
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    mBits = ArrayForBits(bitCount);
    mInt64Count = 0;
    return NOERROR;
}

/* private */
ECode CBitSet::constructor(
    /* [in] */ ArrayOf<Int64>* bits)
{
    mBits = bits;
    mInt64Count = bits->GetLength();
    ShrinkSize();
    return NOERROR;
}

void CBitSet::ShrinkSize()
{
    Int32 i = mInt64Count - 1;
    while (i >= 0 && (*mBits)[i] == 0) {
        --i;
    }
    mInt64Count = i + 1;
}

AutoPtr< ArrayOf<Int64> > CBitSet::ArrayForBits(
    /* [in] */ Int32 bitCount)
{
    AutoPtr< ArrayOf<Int64> > arr = ArrayOf<Int64>::Alloc((bitCount + 63) / 64);
    return arr;
}

ECode CBitSet::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<CBitSet> clone;
    CBitSet::NewByFriend((CBitSet**)&clone);
    clone->mBits = mBits->Clone();
    clone->ShrinkSize();
    *object = (IBitSet*)clone;
    INTERFACE_ADDREF(*object);

    return NOERROR;
}

ECode CBitSet::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (IBitSet::Probe(o) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    if (this == (CBitSet*)IBitSet::Probe(o)) {
        *result = TRUE;
        return NOERROR;
    }
    if (mInt64Count != ((CBitSet *)IBitSet::Probe(o))->mInt64Count) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < mInt64Count; ++i) {
        if ((*mBits)[i] != (*((CBitSet *)IBitSet::Probe(o))->mBits)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

void CBitSet::EnsureCapacity(
    /* [in] */ Int32 desiredInt64Count)
{
    if (desiredInt64Count <= mBits->GetLength()) {
        return;
    }
    Int32 newLength = Elastos::Core::Math::Max(desiredInt64Count, mBits->GetLength() * 2);
    AutoPtr< ArrayOf<Int64> > newBits = ArrayOf<Int64>::Alloc(newLength);
    newBits->Copy(mBits, mInt64Count);
    mBits = newBits;
    // 'longCount' is unchanged by this operation: the long[] is larger,
    // but you're not yet using any more of it.
}

ECode CBitSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    // The RI doesn't use Arrays.hashCode, and explicitly specifies this algorithm.
    Int64 x = 1234;
    for (Int32 i = 0; i < mInt64Count; ++i) {
        x ^= (*mBits)[i] * (i + 1);
    }
    *hashCode = (Int32)((x >> 32) ^ x);
    return NOERROR;
}

/**
 * Returns the bit at index {@code index}. Indexes greater than the current length return false.
 *
 * @throws IndexOutOfBoundsException if {@code index < 0}.
 */
ECode CBitSet::Get(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(CheckIndex(index));
    Int32 arrayIndex = index / 64;
    if (arrayIndex >= mInt64Count) {
        *value = FALSE;
        return NOERROR;
    }
    *value = ((*mBits)[arrayIndex] & (1LL << (index & 0x3F))) != 0;
    return NOERROR;
}

/**
 * Sets the bit at index {@code index} to true.
 *
 * @throws IndexOutOfBoundsException if {@code index < 0}.
 */
ECode CBitSet::Set(
    /* [in] */ Int32 index)
{
    FAIL_RETURN(CheckIndex(index));
    Int32 arrayIndex = index / 64;
    if (arrayIndex >= mBits->GetLength()) {
        EnsureCapacity(arrayIndex + 1);
    }
    (*mBits)[arrayIndex] |= (1LL << (index & 0x3F));
    mInt64Count = Elastos::Core::Math::Max(mInt64Count, arrayIndex + 1);
    return NOERROR;
}

/**
 * Clears the bit at index {@code index}.
 *
 * @throws IndexOutOfBoundsException if {@code index < 0}.
 */
ECode CBitSet::Clear(
    /* [in] */ Int32 index)
{
    FAIL_RETURN(CheckIndex(index));
    Int32 arrayIndex = index / 64;
    if (arrayIndex >= mInt64Count) {
        return NOERROR;
    }
    (*mBits)[arrayIndex] &= ~(1LL << (index & 0x3F));
    ShrinkSize();
    return NOERROR;
}

/**
 * Flips the bit at index {@code index}.
 *
 * @throws IndexOutOfBoundsException if {@code index < 0}.
 */
ECode CBitSet::Flip(
    /* [in] */ Int32 index)
{
    FAIL_RETURN(CheckIndex(index));
    Int32 arrayIndex = index / 64;
    if (arrayIndex >= mBits->GetLength()) {
        EnsureCapacity(arrayIndex + 1);
    }
    (*mBits)[arrayIndex] ^= (1LL << (index & 0x3F));
    mInt64Count = Elastos::Core::Math::Max(mInt64Count, arrayIndex + 1);
    ShrinkSize();
    return NOERROR;
}

ECode CBitSet::CheckIndex(
    /* [in] */ Int32 index)
{
    if (index < 0) {
        // throw new IndexOutOfBoundsException("index < 0: " + index);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode CBitSet::CheckRange(
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    if ((fromIndex | toIndex) < 0 || toIndex < fromIndex) {
        // throw new IndexOutOfBoundsException("fromIndex=" + fromIndex + " toIndex=" + toIndex);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Returns a new {@code BitSet} containing the
 * range of bits {@code [fromIndex, toIndex)}, shifted down so that the bit
 * at {@code fromIndex} is at bit 0 in the new {@code BitSet}.
 *
 * @throws IndexOutOfBoundsException
 *             if {@code fromIndex} or {@code toIndex} is negative, or if
 *             {@code toIndex} is smaller than {@code fromIndex}.
 */
ECode CBitSet::GetEx(
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex,
    /* [out] */ IBitSet** bs)
{
    VALIDATE_NOT_NULL(bs);

    FAIL_RETURN(CheckRange(fromIndex, toIndex));
    Int32 last = 64 * mInt64Count;
    if (fromIndex >= last || fromIndex == toIndex) {
        return CBitSet::New(0, bs);
    }
    if (toIndex > last) {
        toIndex = last;
    }

    Int32 firstArrayIndex = fromIndex / 64;
    Int32 lastArrayIndex = (toIndex - 1) / 64;
    Int64 lowMask = ALL_ONES << (fromIndex & 0x3F);
    Int64 highMask = (UInt64)ALL_ONES >> (-toIndex & 0x3F);

    if (firstArrayIndex == lastArrayIndex) {
        Int64 result = (*mBits)[firstArrayIndex] & (lowMask & highMask);
        result = (UInt64)result >> (fromIndex & 0x3F);
        if (result == 0) {
            return CBitSet::New(0, bs);
        }
        AutoPtr< ArrayOf<Int64> > arr = ArrayOf<Int64>::Alloc(1);
        (*arr)[0] = result;
        return CBitSet::New(arr, bs);
    }

    AutoPtr< ArrayOf<Int64> > newBits = ArrayOf<Int64>::Alloc(lastArrayIndex - firstArrayIndex + 1);

    // first fill in the first and last indexes in the new BitSet
    (*newBits)[0] = (*mBits)[firstArrayIndex] & lowMask;
    (*newBits)[newBits->GetLength() - 1] = (*mBits)[lastArrayIndex] & highMask;

    // fill in the in between elements of the new BitSet
    for (Int32 i = 1; i < lastArrayIndex - firstArrayIndex; i++) {
        (*newBits)[i] = (*mBits)[firstArrayIndex + i];
    }

    // shift all the elements in the new BitSet to the right
    Int32 numBitsToShift = fromIndex % 64;
    Int32 actualLen = newBits->GetLength();
    if (numBitsToShift != 0) {
        for (Int32 i = 0; i < newBits->GetLength(); i++) {
            // shift the current element to the right regardless of
            // sign
            (*newBits)[i] = ((UInt64)(*newBits)[i] >> (numBitsToShift & 0x3F));

            // apply the last x bits of newBits[i+1] to the current
            // element
            if (i != newBits->GetLength() - 1) {
                (*newBits)[i] |= (*newBits)[i + 1] << (-numBitsToShift & 0x3F);
            }
            if ((*newBits)[i] != 0) {
                actualLen = i + 1;
            }
        }
    }
    return CBitSet::New(newBits, bs);
}

/**
 * Sets the bit at index {@code index} to {@code state}.
 *
 * @throws IndexOutOfBoundsException if {@code index < 0}.
 */
ECode CBitSet::SetEx(
    /* [in] */ Int32 index,
    /* [in] */ Boolean state)
{
    if (state) {
        return Set(index);
    }
    else {
        return Clear(index);
    }
}

/**
 * Sets the range of bits {@code [fromIndex, toIndex)} to {@code state}.
 *
 * @throws IndexOutOfBoundsException
 *             if {@code fromIndex} or {@code toIndex} is negative, or if
 *             {@code toIndex} is smaller than {@code fromIndex}.
 */
ECode CBitSet::SetEx3(
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex,
    /* [in] */ Boolean state)
{
    if (state) {
        return SetEx2(fromIndex, toIndex);
    }
    else {
        return ClearEx(fromIndex, toIndex);
    }
}

/**
 * Clears all the bits in this {@code BitSet}. This method does not change the capacity.
 * Use {@code clear} if you want to reuse this {@code BitSet} with the same capacity, but
 * create a new {@code BitSet} if you're trying to potentially reclaim memory.
 */
ECode CBitSet::ClearEx3()
{
    for (Int32 i = 0; i < mInt64Count; ++i) {
        (*mBits)[i] = 0LL;
    }
    mInt64Count = 0;
    return NOERROR;
}

/**
 * Sets the range of bits {@code [fromIndex, toIndex)}.
 *
 * @throws IndexOutOfBoundsException
 *             if {@code fromIndex} or {@code toIndex} is negative, or if
 *             {@code toIndex} is smaller than {@code fromIndex}.
 */
ECode CBitSet::SetEx2(
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    FAIL_RETURN(CheckRange(fromIndex, toIndex));
    if (fromIndex == toIndex) {
        return NOERROR;
    }
    Int32 firstArrayIndex = fromIndex / 64;
    Int32 lastArrayIndex = (toIndex - 1) / 64;
    if (lastArrayIndex >= mBits->GetLength()) {
        EnsureCapacity(lastArrayIndex + 1);
    }

    Int64 lowMask = ALL_ONES << (fromIndex & 0x3F);
    Int64 highMask = (UInt64)ALL_ONES >> (-toIndex & 0x3F);
    if (firstArrayIndex == lastArrayIndex) {
        (*mBits)[firstArrayIndex] |= (lowMask & highMask);
    }
    else {
        Int32 i = firstArrayIndex;
        (*mBits)[i++] |= lowMask;
        while (i < lastArrayIndex) {
            (*mBits)[i++] |= ALL_ONES;
        }
        (*mBits)[i++] |= highMask;
    }
    mInt64Count = Elastos::Core::Math::Max(mInt64Count, lastArrayIndex + 1);
    return NOERROR;
}

/**
 * Clears the range of bits {@code [fromIndex, toIndex)}.
 *
 * @throws IndexOutOfBoundsException
 *             if {@code fromIndex} or {@code toIndex} is negative, or if
 *             {@code toIndex} is smaller than {@code fromIndex}.
 */
ECode CBitSet::ClearEx(
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    FAIL_RETURN(CheckRange(fromIndex, toIndex));
    if (fromIndex == toIndex || mInt64Count == 0) {
        return NOERROR;
    }
    Int32 last = 64 * mInt64Count;
    if (fromIndex >= last) {
        return NOERROR;
    }
    if (toIndex > last) {
        toIndex = last;
    }
    Int32 firstArrayIndex = fromIndex / 64;
    Int32 lastArrayIndex = (toIndex - 1) / 64;

    Int64 lowMask = ALL_ONES << (fromIndex & 0x3F);
    Int64 highMask = (UInt64)ALL_ONES >> (-toIndex & 0x3F);
    if (firstArrayIndex == lastArrayIndex) {
        (*mBits)[firstArrayIndex] &= ~(lowMask & highMask);
    }
    else {
        Int32 i = firstArrayIndex;
        (*mBits)[i++] &= ~lowMask;
        while (i < lastArrayIndex) {
            (*mBits)[i++] = 0LL;
        }
        (*mBits)[i++] &= ~highMask;
    }
    ShrinkSize();
    return NOERROR;
}

/**
 * Flips the range of bits {@code [fromIndex, toIndex)}.
 *
 * @throws IndexOutOfBoundsException
 *             if {@code fromIndex} or {@code toIndex} is negative, or if
 *             {@code toIndex} is smaller than {@code fromIndex}.
 */
ECode CBitSet::FlipEx(
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    FAIL_RETURN(CheckRange(fromIndex, toIndex));
    if (fromIndex == toIndex) {
        return NOERROR;
    }
    Int32 firstArrayIndex = fromIndex / 64;
    Int32 lastArrayIndex = (toIndex - 1) / 64;
    if (lastArrayIndex >= mBits->GetLength()) {
        EnsureCapacity(lastArrayIndex + 1);
    }

    Int64 lowMask = ALL_ONES << (fromIndex & 0x3F);
    Int64 highMask = (UInt64)ALL_ONES >> (-toIndex & 0x3F);
    if (firstArrayIndex == lastArrayIndex) {
        (*mBits)[firstArrayIndex] ^= (lowMask & highMask);
    }
    else {
        Int32 i = firstArrayIndex;
        (*mBits)[i++] ^= lowMask;
        while (i < lastArrayIndex) {
            (*mBits)[i++] ^= ALL_ONES;
        }
        (*mBits)[i++] ^= highMask;
    }
    mInt64Count = Elastos::Core::Math::Max(mInt64Count, lastArrayIndex + 1);
    ShrinkSize();
    return NOERROR;
}

/**
 * Returns true if {@code this.and(bs)} is non-empty, but may be faster than computing that.
 */
ECode CBitSet::Intersects(
    /* [in] */ IBitSet* bs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr< ArrayOf<Int64> > bsBits = ((CBitSet*)bs)->mBits;
    Int32 length = Elastos::Core::Math::Min(mInt64Count, ((CBitSet*)bs)->mInt64Count);
    for (Int32 i = 0; i < length; ++i) {
        if (((*mBits)[i] & (*bsBits)[i]) != 0LL) {
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;
    return NOERROR;
}

/**
 * Logically ands the bits of this {@code BitSet} with {@code bs}.
 */
ECode CBitSet::And(
    /* [in] */ IBitSet* bs)
{
    CBitSet* bsObj = (CBitSet*)bs;
    Int32 minSize = Elastos::Core::Math::Min(mInt64Count, bsObj->mInt64Count);
    for (Int32 i = 0; i < minSize; ++i) {
        (*mBits)[i] &= (*bsObj->mBits)[i];
    }
    for (Int32 i = minSize; i < mInt64Count; ++i) {
        (*mBits)[i] = 0LL;
    }
    ShrinkSize();
    return NOERROR;
}

/**
 * Clears all bits in this {@code BitSet} which are also set in {@code bs}.
 */
ECode CBitSet::AndNot(
    /* [in] */ IBitSet* bs)
{
    CBitSet* bsObj = (CBitSet*)bs;
    Int32 minSize = Elastos::Core::Math::Min(mInt64Count, bsObj->mInt64Count);
    for (Int32 i = 0; i < minSize; ++i) {
        (*mBits)[i] &= ~(*bsObj->mBits)[i];
    }
    ShrinkSize();
    return NOERROR;
}

/**
 * Logically ors the bits of this {@code BitSet} with {@code bs}.
 */
ECode CBitSet::Or(
    /* [in] */ IBitSet* bs)
{
    CBitSet* bsObj = (CBitSet*)bs;
    Int32 minSize = Elastos::Core::Math::Min(mInt64Count, bsObj->mInt64Count);
    Int32 maxSize = Elastos::Core::Math::Max(mInt64Count, bsObj->mInt64Count);
    EnsureCapacity(maxSize);
    for (Int32 i = 0; i < minSize; ++i) {
        (*mBits)[i] |= (*bsObj->mBits)[i];
    }
    if (bsObj->mInt64Count > minSize) {
        mBits->Copy(minSize, bsObj->mBits, minSize, maxSize - minSize);
    }
    mInt64Count = maxSize;
    return NOERROR;
}

/**
 * Logically xors the bits of this {@code BitSet} with {@code bs}.
 */
ECode CBitSet::Xor(
    /* [in] */ IBitSet* bs)
{
    CBitSet* bsObj = (CBitSet*)bs;
    Int32 minSize = Elastos::Core::Math::Min(mInt64Count, bsObj->mInt64Count);
    Int32 maxSize = Elastos::Core::Math::Max(mInt64Count, bsObj->mInt64Count);
    EnsureCapacity(maxSize);
    for (Int32 i = 0; i < minSize; ++i) {
        (*mBits)[i] ^= (*bsObj->mBits)[i];
    }
    if (bsObj->mInt64Count > minSize) {
        mBits->Copy(minSize, bsObj->mBits, minSize, maxSize - minSize);
    }
    mInt64Count = maxSize;
    ShrinkSize();
    return NOERROR;
}

/**
 * Returns the capacity in bits of the array implementing this {@code BitSet}. This is
 * unrelated to the length of the {@code BitSet}, and not generally useful.
 * Use {@link #nextSetBit} to iterate, or {@link #length} to find the highest set bit.
 */
ECode CBitSet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
       *size = mBits->GetLength() * 64;
       return NOERROR;
}

/**
 * Returns the number of bits up to and including the highest bit set. This is unrelated to
 * the {@link #size} of the {@code BitSet}.
 */
ECode CBitSet::GetLength(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);
    if (mInt64Count == 0) {
        *len = 0;
        return NOERROR;
    }
    *len = 64 * (mInt64Count - 1) +
            (64 - Elastos::Core::Math::NumberOfLeadingZeros((*mBits)[mInt64Count - 1]));
    return NOERROR;
}

/**
 * Returns the index of the first bit that is set on or after {@code index}, or -1
 * if no higher bits are set.
 * @throws IndexOutOfBoundsException if {@code index < 0}.
 */
ECode CBitSet::NextSetBit(
    /* [in] */ Int32 index,
    /* [out] */ Int32* next)
{
    VALIDATE_NOT_NULL(next);
    FAIL_RETURN(CheckIndex(index));
    Int32 arrayIndex = index / 64;
    if (arrayIndex >= mInt64Count) {
        *next = -1;
        return NOERROR;
    }
    Int64 mask = ALL_ONES << (index & 0x3F);
    if (((*mBits)[arrayIndex] & mask) != 0) {
        *next = 64 * arrayIndex +
                Elastos::Core::Math::NumberOfTrailingZeros((*mBits)[arrayIndex] & mask);
        return NOERROR;
    }
    while (++arrayIndex < mInt64Count && (*mBits)[arrayIndex] == 0) {
    }
    if (arrayIndex == mInt64Count) {
        *next = -1;
        return NOERROR;
    }
    *next = 64 * arrayIndex +
            Elastos::Core::Math::NumberOfTrailingZeros((*mBits)[arrayIndex]);
    return NOERROR;
}

/**
 * Returns the index of the first bit that is clear on or after {@code index}.
 * Since all bits past the end are implicitly clear, this never returns -1.
 * @throws IndexOutOfBoundsException if {@code index < 0}.
 */
ECode CBitSet::NextClearBit(
    /* [in] */ Int32 index,
    /* [out] */ Int32* next)
{
    VALIDATE_NOT_NULL(next);
    FAIL_RETURN(CheckIndex(index));
    Int32 arrayIndex = index / 64;
    if (arrayIndex >= mInt64Count) {
        *next = index;
        return NOERROR;
    }
    Int64 mask = ALL_ONES << (index & 0x3F);
    if ((~(*mBits)[arrayIndex] & mask) != 0) {
        *next = 64 * arrayIndex +
                Elastos::Core::Math::NumberOfTrailingZeros(~(*mBits)[arrayIndex] & mask);
        return NOERROR;
    }
    while (++arrayIndex < mInt64Count && (*mBits)[arrayIndex] == ALL_ONES) {
    }
    if (arrayIndex == mInt64Count) {
        *next = 64 * mInt64Count;
        return NOERROR;
    }
    *next = 64 * arrayIndex +
            Elastos::Core::Math::NumberOfTrailingZeros(~(*mBits)[arrayIndex]);
    return NOERROR;
}

/**
 * Returns the index of the first bit that is set on or before {@code index}, or -1 if
 * no lower bits are set or {@code index == -1}.
 * @throws IndexOutOfBoundsException if {@code index < -1}.
 * @hide 1.7
 */
ECode CBitSet::PreviousSetBit(
    /* [in] */ Int32 index,
    /* [out] */ Int32* previous)
{
    VALIDATE_NOT_NULL(previous);
    if (index == -1) {
        *previous = -1;
        return NOERROR;
    }
    FAIL_RETURN(CheckIndex(index));
    // TODO: optimize this.
    for (Int32 i = index; i >= 0; --i) {
        Boolean value;
        if (Get(i, &value), value) {
            *previous = i;
            return NOERROR;
        }
    }
    *previous = -1;
    return NOERROR;
}

/**
 * Returns the index of the first bit that is clear on or before {@code index}, or -1 if
 * no lower bits are clear or {@code index == -1}.
 * @throws IndexOutOfBoundsException if {@code index < -1}.
 * @hide 1.7
 */
ECode CBitSet::PreviousClearBit(
    /* [in] */ Int32 index,
    /* [out] */ Int32* previous)
{
    VALIDATE_NOT_NULL(previous);
    if (index == -1) {
        *previous = -1;
        return NOERROR;
    }
    FAIL_RETURN(CheckIndex(index));
    // TODO: optimize this.
    for (Int32 i = index; i >= 0; --i) {
        Boolean value;
        if (Get(i, &value), !value) {
            *previous = i;
            return NOERROR;
        }
    }
    *previous = -1;
    return NOERROR;
}

/**
 * Returns true if all the bits in this {@code BitSet} are set to false, false otherwise.
 */
ECode CBitSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mInt64Count == 0);
    return NOERROR;
}

/**
 * Returns the number of bits that are {@code true} in this {@code BitSet}.
 */
ECode CBitSet::Cardinality(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    Int32 result = 0;
    for (Int32 i = 0; i < mInt64Count; ++i) {
        result += Elastos::Core::Math::BitCount((*mBits)[i]);
    }
    *number = result;
    return NOERROR;
}

/**
 * Equivalent to {@code BitSet.valueOf(LongBuffer.wrap(longs))}, but likely to be faster.
 * This is likely to be the fastest way to create a {@code BitSet} because it's closest
 * to the internal representation.
 * @hide 1.7
 */
ECode CBitSet::ValueOfInt64Array(
    /* [in] */ const ArrayOf<Int64>& int64Arr,
    /* [out] */ IBitSet** bs)
{
    VALIDATE_NOT_NULL(bs);
    return CBitSet::New(int64Arr.Clone(), bs);
}

/**
 * Returns a {@code BitSet} corresponding to {@code longBuffer}, interpreted as a little-endian
 * sequence of bits. This method does not alter the {@code LongBuffer}.
 * @hide 1.7
 */
ECode CBitSet::ValueOfInt64Buffer(
    /* [in] */ IInt64Buffer* int64Arr,
    /* [out] */ IBitSet** bs)
{
    VALIDATE_NOT_NULL(bs);
    // The bulk get would mutate LongBuffer (even if we reset position later), and it's not
    // clear that's allowed. My assumption is that it's the long[] variant that's the common
    // case anyway, so copy the buffer into a long[].
    Int32 size = 0;
    int64Arr->GetRemaining(&size);
    AutoPtr<ArrayOf<Int64> > int64s = ArrayOf<Int64>::Alloc(size);
    Int32 position;
    int64Arr->GetPosition(&position);
    for (Int32 i = 0; i < size; ++i) {
        Int64 value;
        int64Arr->GetInt64Ex(position + i, &value);
        (*int64s)[i] = value;
    }
    return ValueOfInt64Array(*int64s, bs);
}

/**
 * Equivalent to {@code BitSet.valueOf(ByteBuffer.wrap(bytes))}.
 * @hide 1.7
 */
ECode CBitSet::ValueOfByteArray(
    /* [in] */ const ArrayOf<Byte>& byteArr,
    /* [out] */ IBitSet** bs)
{
    VALIDATE_NOT_NULL(bs);

    AutoPtr<IByteBufferHelper> helper;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
    AutoPtr<IByteBuffer> byteBuffer;
    helper->WrapArray((ArrayOf<Byte>*)&byteArr, (IByteBuffer**)&byteBuffer);
    return ValueOfByteBuffer(byteBuffer, bs);
}

/**
 * Returns a {@code BitSet} corresponding to {@code byteBuffer}, interpreted as a little-endian
 * sequence of bits. This method does not alter the {@code ByteBuffer}.
 * @hide 1.7
 */
ECode CBitSet::ValueOfByteBuffer(
    /* [in] */ IByteBuffer* byteArr,
    /* [out] */ IBitSet** bs)
{
    VALIDATE_NOT_NULL(byteArr);
    VALIDATE_NOT_NULL(bs);

    AutoPtr<IByteBuffer> byteBuffer;
    byteArr->Slice((IByteBuffer**)&byteBuffer);
    byteBuffer->SetOrder(Elastos::IO::ByteOrder_LITTLE_ENDIAN);
    Int32 remaining;
    byteBuffer->GetRemaining(&remaining);
    AutoPtr< ArrayOf<Int64> > longs = ArrayForBits(remaining * 8);
    Int32 i = 0;
    Int64 lvalue = 0;
    while(remaining >= sizeof(Int64)) {
        byteBuffer->GetInt64(&lvalue);
        (*longs)[i++] = lvalue;
        byteBuffer->GetRemaining(&remaining);
    }
    Boolean hasRemaining;
    byteBuffer->HasRemaining(&hasRemaining);
    for (Int32 j = 0; hasRemaining; ++j) {
        Byte byte;
        byteBuffer->GetByte(&byte);
        (*longs)[i] |= (((Int64)byte & 0xff << ((8 * j) & 0x3F)));

        byteBuffer->HasRemaining(&hasRemaining);
    }

   return CBitSet::ValueOfInt64Array(*longs, bs);
}

/**
 * Returns a new {@code long[]} containing a little-endian representation of the bits of
 * this {@code BitSet}, suitable for passing to {@code valueOf} to reconstruct
 * this {@code BitSet}.
 * @hide 1.7
 */
ECode CBitSet::ToInt64Array(
    /* [out, callee] */ ArrayOf<Int64>** int64Arr)
{
    VALIDATE_NOT_NULL(int64Arr);
    AutoPtr< ArrayOf<Int64> > arr = ArrayOf<Int64>::Alloc(mInt64Count);
    arr->Copy(mBits, mInt64Count);
    *int64Arr = arr;
    ARRAYOF_ADDREF(*int64Arr);
    return NOERROR;
}

/**
 * Returns a new {@code byte[]} containing a little-endian representation the bits of
 * this {@code BitSet}, suitable for passing to {@code valueOf} to reconstruct
 * this {@code BitSet}.
 * @hide 1.7
 */
ECode CBitSet::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** byteArr)
{
    VALIDATE_NOT_NULL(byteArr);
    Int32 bitCount;
    GetLength(&bitCount);
    AutoPtr< ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc((bitCount + 7) / 8);
    for (Int32 i = 0; i < result->GetLength(); ++i) {
        Int32 lowBit = 8 * i;
        Int32 arrayIndex = lowBit / 64;
        (*result)[i] = (Byte)(((UInt64)(*mBits)[arrayIndex]) >> (lowBit & 0x3F));
    }
    *byteArr = result;
    ARRAYOF_ADDREF(*byteArr);
    return NOERROR;
}

ECode CBitSet::ToString(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    //System.err.println("BitSet[longCount=" + longCount + ",bits=" + Arrays.toString(bits) + "]");
    StringBuilder sb;
    sb.AppendChar('{');
    Boolean comma = FALSE;
    for (Int32 i = 0; i < mInt64Count; ++i) {
        if ((*mBits)[i] != 0) {
            for (Int32 j = 0; j < 64; ++j) {
                if (((*mBits)[i] & 1LL << j) != 0) {
                    if (comma) {
                        sb.AppendString(String(", "));
                    } else {
                        comma = TRUE;
                    }
                    sb.AppendInt32(64 * i + j);
                }
            }
        }
    }
    sb.AppendChar('}');
    *value = sb.ToString();
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
