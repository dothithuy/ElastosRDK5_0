#ifndef __UTILITY_CARRAYS_H__
#define __UTILITY_CARRAYS_H__

#include "_CArrays.h"
#include "AbstractList.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::IComparator;
using Elastos::Core::StringBuilder;
using Elastos::IO::ISerializable;

namespace Elastos {
namespace Utility {

CarClass(CArrays)
{
private:
    class ArrayList
        : public AbstractList
        , public IList
        , public ISerializable
        , public IRandomAccess
        , public ElRefBase
    {
    public:
        ArrayList(
            /* [in] */ ArrayOf<IInterface* >* storage);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *object,
            /* [out] */ InterfaceID *pIID);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** result);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* result);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out] */ ArrayOf<IInterface* >** outArray);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface* >* contents,
            /* [out] */ ArrayOf<IInterface* >** outArray);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* res);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* res);

        CARAPI GetHashCode(
            /* [in] */ Int32* hashCode);

        CARAPI IsEmpty(
            /* [out] */ Boolean* res);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI AddEx(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object);

        CARAPI AddAllEx(
            /* [in] */ Int32 location,
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetListIterator(
            /* [out] */ IListIterator** it);

        CARAPI GetListIteratorEx(
            /* [in] */ Int32 location,
            /* [out] */ IListIterator** it);

        CARAPI RemoveEx(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

    private:
        AutoPtr<ArrayOf<IInterface*> > mA;
    };

public:
    CARAPI AsList(
        /* [in] */ ArrayOf<IInterface*>* array,
        /* [out] */ IList** outList);

    CARAPI BinarySearchByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchByteEx(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Char32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchChar32Ex(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Char32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Double value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchDoubleEx(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Double value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Float value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchFloatEx(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Float value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt32Ex(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int64 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt64Ex(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int64 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchObjectEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface* value,
        /* [out] */ Int32* index);

    CARAPI BinarySearch(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    CARAPI BinarySearchEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface* value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int16 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt16Ex(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int16 value,
        /* [out] */ Int32* index);

    CARAPI FillByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Byte value);

    CARAPI FillByteEx(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Byte value);

    CARAPI FillInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int16 value);

    CARAPI FillInt16Ex(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int16 value);

    CARAPI FillChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Char32 value);

    CARAPI FillChar32Ex(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Char32 value);

    CARAPI FillInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 value);

    CARAPI FillInt32Ex(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 value);

    CARAPI FillInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int64 value);

    CARAPI FillInt64Ex(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int64 value);

    CARAPI FillFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Float value);

    CARAPI FillFloatEx(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float value);

    CARAPI FillDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Double value);

    CARAPI FillDoubleEx(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Double value);

    CARAPI FillBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [in] */ Boolean value);

    CARAPI FillBooleanEx(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean value);

    CARAPI FillObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value);

    CARAPI FillObjectEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IInterface* value);

    CARAPI HashCodeBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ Int32* code);

    CARAPI DeepHashCode(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ Int32* code);

    CARAPI EqualsByte(
        /* [in] */ ArrayOf<Byte>* array1,
        /* [in] */ ArrayOf<Byte>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsInt16(
        /* [in] */ ArrayOf<Int16>* array1,
        /* [in] */ ArrayOf<Int16>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsChar32(
        /* [in] */ ArrayOf<Char32>* array1,
        /* [in] */ ArrayOf<Char32>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsInt32(
        /* [in] */ ArrayOf<Int32>* array1,
        /* [in] */ ArrayOf<Int32>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsInt64(
        /* [in] */ ArrayOf<Int64>* array1,
        /* [in] */ ArrayOf<Int64>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsFloat(
        /* [in] */ ArrayOf<Float>* array1,
        /* [in] */ ArrayOf<Float>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsDouble(
        /* [in] */ ArrayOf<Double>* array1,
        /* [in] */ ArrayOf<Double>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsBoolean(
        /* [in] */ ArrayOf<Boolean>* array1,
        /* [in] */ ArrayOf<Boolean>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsObject(
        /* [in] */ ArrayOf<IInterface* >* array1,
        /* [in] */ ArrayOf<IInterface* >* array2,
        /* [out] */ Boolean* result);

    CARAPI DeepEquals(
        /* [in] */ ArrayOf<IInterface* >* array1,
        /* [in] */ ArrayOf<IInterface* >* array2,
        /* [out] */ Boolean* result);

    CARAPI SortByte(
        /* [in] */ ArrayOf<Byte>* array);

    CARAPI SortByteEx(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckOffsetAndCount(
        /* [in] */ Int32 arrayLength,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckStartAndEnd(
        /* [in] */ Int32 len,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortChar32(
        /* [in] */ ArrayOf<Char32>* array);

    CARAPI SortChar32Ex(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortDouble(
        /* [in] */ ArrayOf<Double>* array);

    CARAPI SortDoubleEx(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortFloat(
        /* [in] */ ArrayOf<Float>* array);

    CARAPI SortFloatEx(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortInt32(
        /* [in] */ ArrayOf<Int32>* array);

    CARAPI SortInt32Ex(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortInt64(
        /* [in] */ ArrayOf<Int64>* array);

    CARAPI SortInt64Ex(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortInt16(
        /* [in] */ ArrayOf<Int16>* array);

    CARAPI SortInt16Ex(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortObject(
        /* [in] */ ArrayOf<IInterface* >* array);

    CARAPI SortObjectEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IComparator* comparator);

    CARAPI Sort(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IComparator* comparator);

    CARAPI ToStringBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [out] */ String* str);

    CARAPI ToStringByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [out] */ String* str);

    CARAPI ToStringChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [out] */ String* str);

    CARAPI ToStringDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ String* str);

    CARAPI ToStringFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ String* str);

    CARAPI ToStringInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [out] */ String* str);

    CARAPI ToStringInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ String* str);

    CARAPI ToStringInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [out] */ String* str);

    CARAPI ToStringObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ String* str);

    CARAPI DeepToString(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ String* str);

    CARAPI CopyOfBoolean(
        /* [in] */ ArrayOf<Boolean>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Boolean>** result);

    CARAPI CopyOfByte(
        /* [in] */ ArrayOf<Byte>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI CopyOfChar32(
        /* [in] */ ArrayOf<Char32>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Char32>** result);

    CARAPI CopyOfDouble(
        /* [in] */ ArrayOf<Double>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Double>** result);

    CARAPI CopyOfFloat(
        /* [in] */ ArrayOf<Float>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Float>** result);

    CARAPI CopyOfInt32(
        /* [in] */ ArrayOf<Int32>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int32>** result);

    CARAPI CopyOfInt64(
        /* [in] */ ArrayOf<Int64>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int64>** result);

    CARAPI CopyOfInt16(
        /* [in] */ ArrayOf<Int16>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int16>** result);

    CARAPI CopyOf(
        /* [in] */ ArrayOf<IInterface* >* original,
        /* [in] */ int newLength,
        /* [out, callee] */ ArrayOf<IInterface* >** result);

    // CARAPI CopyOfEx(
    //     /* [in] */ U[] original,
    //     /* [in] */ int newLength,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] result);

    CARAPI CopyOfRangeBoolean(
        /* [in] */ ArrayOf<Boolean>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Boolean>** arrayCopy);

    CARAPI CopyOfRangeByte(
        /* [in] */ ArrayOf<Byte>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Byte>** arrayCopy);

    CARAPI CopyOfRangeChar32(
        /* [in] */ ArrayOf<Char32>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Char32>** arrayCopy);

    CARAPI CopyOfRangeDouble(
        /* [in] */ ArrayOf<Double>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Double>** arrayCopy);

    CARAPI CopyOfRangeFloat(
        /* [in] */ ArrayOf<Float>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Float>** arrayCopy);

    CARAPI CopyOfRangeInt32(
        /* [in] */ ArrayOf<Int32>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int32>** arrayCopy);

    CARAPI CopyOfRangeInt64(
        /* [in] */ ArrayOf<Int64>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int64>** arrayCopy);

    CARAPI CopyOfRangeInt16(
        /* [in] */ ArrayOf<Int16>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int16>** arrayCopy);

    CARAPI CopyOfRange(
        /* [in] */ ArrayOf<IInterface* >* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<IInterface* >** arrayCopy);

    // static CARAPI CopyOfRangeEx(
    //     /* [in] */ U[] original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] arrayCopy);

public:
    static CARAPI _AsList(
        /* [in] */ ArrayOf<IInterface*>* array,
        /* [out] */ IList** outList);

    static CARAPI _BinarySearchByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchByteEx(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Char32 value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchChar32Ex(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Char32 value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Double value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchDoubleEx(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Double value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Float value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchFloatEx(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Float value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchInt32Ex(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int32 value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int64 value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchInt64Ex(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int64 value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchObjectEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface* value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearch(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface* value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int16 value,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchInt16Ex(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int16 value,
        /* [out] */ Int32* index);

    static CARAPI _FillByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Byte value);

    static CARAPI _FillByteEx(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Byte value);

    static CARAPI _FillInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int16 value);

    static CARAPI _FillInt16Ex(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int16 value);

    static CARAPI _FillChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Char32 value);

    static CARAPI _FillChar32Ex(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Char32 value);

    static CARAPI _FillInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 value);

    static CARAPI _FillInt32Ex(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 value);

    static CARAPI _FillInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int64 value);

    static CARAPI _FillInt64Ex(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int64 value);

    static CARAPI _FillFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Float value);

    static CARAPI _FillFloatEx(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float value);

    static CARAPI _FillDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Double value);

    static CARAPI _FillDoubleEx(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Double value);

    static CARAPI _FillBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [in] */ Boolean value);

    static CARAPI _FillBooleanEx(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean value);

    static CARAPI _FillObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value);

    static CARAPI _FillObjectEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IInterface* value);

    static CARAPI _HashCodeBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ Int32* code);

    static CARAPI _HashCodeObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ Int32* code);

    static CARAPI _DeepHashCode(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ Int32* code);

    static CARAPI _EqualsByte(
        /* [in] */ ArrayOf<Byte>* array1,
        /* [in] */ ArrayOf<Byte>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsInt16(
        /* [in] */ ArrayOf<Int16>* array1,
        /* [in] */ ArrayOf<Int16>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsChar32(
        /* [in] */ ArrayOf<Char32>* array1,
        /* [in] */ ArrayOf<Char32>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsInt32(
        /* [in] */ ArrayOf<Int32>* array1,
        /* [in] */ ArrayOf<Int32>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsInt64(
        /* [in] */ ArrayOf<Int64>* array1,
        /* [in] */ ArrayOf<Int64>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsFloat(
        /* [in] */ ArrayOf<Float>* array1,
        /* [in] */ ArrayOf<Float>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsDouble(
        /* [in] */ ArrayOf<Double>* array1,
        /* [in] */ ArrayOf<Double>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsBoolean(
        /* [in] */ ArrayOf<Boolean>* array1,
        /* [in] */ ArrayOf<Boolean>* array2,
        /* [out] */ Boolean* result);

    static CARAPI _EqualsObject(
        /* [in] */ ArrayOf<IInterface* >* array1,
        /* [in] */ ArrayOf<IInterface* >* array2,
        /* [out] */ Boolean* result);

    static CARAPI _DeepEquals(
        /* [in] */ ArrayOf<IInterface* >* array1,
        /* [in] */ ArrayOf<IInterface* >* array2,
        /* [out] */ Boolean* result);

    static CARAPI _SortByte(
        /* [in] */ ArrayOf<Byte>* array);

    static CARAPI _SortByteEx(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _CheckOffsetAndCount(
        /* [in] */ Int32 arrayLength,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    static CARAPI _CheckStartAndEnd(
        /* [in] */ Int32 len,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortChar32(
        /* [in] */ ArrayOf<Char32>* array);

    static CARAPI _SortChar32Ex(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortDouble(
        /* [in] */ ArrayOf<Double>* array);

    static CARAPI _SortDoubleEx(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortFloat(
        /* [in] */ ArrayOf<Float>* array);

    static CARAPI _SortFloatEx(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortInt32(
        /* [in] */ ArrayOf<Int32>* array);

    static CARAPI _SortInt32Ex(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortInt64(
        /* [in] */ ArrayOf<Int64>* array);

    static CARAPI _SortInt64Ex(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortInt16(
        /* [in] */ ArrayOf<Int16>* array);

    static CARAPI _SortInt16Ex(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortObject(
        /* [in] */ ArrayOf<IInterface* >* array);

    static CARAPI _SortObjectEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI _SortEx(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IComparator* comparator);

    static CARAPI _Sort(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IComparator* comparator);

    static CARAPI _ToStringBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [out] */ String* str);

    static CARAPI _ToStringObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ String* str);

    static CARAPI _DeepToString(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ String* str);

    static CARAPI _CopyOfBoolean(
        /* [in] */ ArrayOf<Boolean>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Boolean>** result);

    static CARAPI _CopyOfByte(
        /* [in] */ ArrayOf<Byte>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI _CopyOfChar32(
        /* [in] */ ArrayOf<Char32>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Char32>** result);

    static CARAPI _CopyOfDouble(
        /* [in] */ ArrayOf<Double>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Double>** result);

    static CARAPI _CopyOfFloat(
        /* [in] */ ArrayOf<Float>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Float>** result);

    static CARAPI _CopyOfInt32(
        /* [in] */ ArrayOf<Int32>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int32>** result);

    static CARAPI _CopyOfInt64(
        /* [in] */ ArrayOf<Int64>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int64>** result);

    static CARAPI _CopyOfInt16(
        /* [in] */ ArrayOf<Int16>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int16>** result);

    static CARAPI _CopyOf(
        /* [in] */ ArrayOf<IInterface* >* original,
        /* [in] */ int newLength,
        /* [out, callee] */ ArrayOf<IInterface* >** result);

    // static CARAPI _CopyOfEx(
    //     /* [in] */ U[] original,
    //     /* [in] */ int newLength,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] result);

    static CARAPI _CopyOfRangeBoolean(
        /* [in] */ ArrayOf<Boolean>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Boolean>** arrayCopy);

    static CARAPI _CopyOfRangeByte(
        /* [in] */ ArrayOf<Byte>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Byte>** arrayCopy);

    static CARAPI _CopyOfRangeChar32(
        /* [in] */ ArrayOf<Char32>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Char32>** arrayCopy);

    static CARAPI _CopyOfRangeDouble(
        /* [in] */ ArrayOf<Double>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Double>** arrayCopy);

    static CARAPI _CopyOfRangeFloat(
        /* [in] */ ArrayOf<Float>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Float>** arrayCopy);

    static CARAPI _CopyOfRangeInt32(
        /* [in] */ ArrayOf<Int32>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int32>** arrayCopy);

    static CARAPI _CopyOfRangeInt64(
        /* [in] */ ArrayOf<Int64>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int64>** arrayCopy);

    static CARAPI _CopyOfRangeInt16(
        /* [in] */ ArrayOf<Int16>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int16>** arrayCopy);

    static CARAPI _CopyOfRange(
        /* [in] */ ArrayOf<IInterface* >* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<IInterface* >** arrayCopy);

    // static CARAPI _CopyOfRangeEx(
    //     /* [in] */ U[] original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] arrayCopy);

private:
//    CArrays();

    static CARAPI CheckBinarySearchBounds(
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int32 length);

    static CARAPI_(Int32) DeepHashCodeElement(
        /* [in] */ IInterface* element);

    static CARAPI_(Boolean) DeepEqualsElements(
        /* [in] */ IInterface* e1,
        /* [in] */ IInterface* e2);

    static CARAPI_(void) DeepToStringImpl(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ ArrayOf<IInterface* >* origArrays,
        /* [in] */ StringBuilder* sb);

    static CARAPI_(Boolean) DeepToStringImplContains(
        /* [in] */ ArrayOf<IInterface* >* origArrays,
        /* [in] */ IInterface* array);

};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CARRAYS_H__