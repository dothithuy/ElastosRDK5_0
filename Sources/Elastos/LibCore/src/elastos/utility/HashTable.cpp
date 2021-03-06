
#include "CHashTable.h"
#include "CFloat.h"
#include "elastos/StringBuilder.h"
#include "elastos/ObjectUtils.h"
#include <cutils/log.h>

using Elastos::Core::ObjectUtils;
using Elastos::Core::IFloat;
using Elastos::Core::CFloat;
using Elastos::Core::StringBuilder;
using Elastos::IO::IObjectOutputStreamPutField;

namespace Elastos {
namespace Utility {

//==========================================================
//       HashTable::HashtableEntry
//==========================================================
CAR_INTERFACE_IMPL(HashTable::HashtableEntry, IMapEntry);

HashTable::HashtableEntry::HashtableEntry(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 hash,
    /* [in] */ HashtableEntry* next)
{
    mKey = key;
    mValue = value;
    mHash = hash;
    mNext = next;
}

ECode HashTable::HashtableEntry::GetKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    *outface = mKey;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode HashTable::HashtableEntry::GetValue(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    *outface = mValue;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode HashTable::HashtableEntry::SetValue(
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    if (mValue == NULL) {
        // throw new NullPointerException("value == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IInterface> oldValue = mValue;
    mValue = value;
    *outface = oldValue;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode HashTable::HashtableEntry::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (!(IMapEntry::Probe(o))) {
        *value = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    AutoPtr<IInterface> valueface;
    e->GetKey((IInterface**)&keyface);
    e->GetValue((IInterface**)&valueface);
    *value = ObjectUtils::Equals(mKey, keyface) && ObjectUtils::Equals(mValue, valueface);
    return NOERROR;
}

ECode HashTable::HashtableEntry::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Int32 keycode = ObjectUtils::GetHashCode(mKey);
    Int32 valuecode = ObjectUtils::GetHashCode(mValue);
    *value = keycode ^ valuecode;
    return NOERROR;
}

ECode HashTable::HashtableEntry::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = ObjectUtils::ToString(mKey) + String("=") + ObjectUtils::ToString(mValue);
    return NOERROR;
}


//==========================================================
//       HashTable::HashIterator
//==========================================================
HashTable::HashIterator::HashIterator()
{
}

HashTable::HashIterator::HashIterator(
    /* [in] */ HashTable* host)
{
    mHost = host;
    mNextIndex = 0;
    mExpectedModCount = mHost->mModCount;
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mHost->mTable;
    AutoPtr<HashtableEntry> next;
    while (next == NULL && mNextIndex < tab->GetLength()) {
        next = (*tab)[mNextIndex++];
    }
    mNextEntry = next;
}

ECode HashTable::HashIterator::HasNext(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mNextEntry != NULL;
    return NOERROR;
}

AutoPtr<HashTable::HashtableEntry> HashTable::HashIterator::NextEntry()
{
    if (mHost->mModCount != mExpectedModCount) {
        // throw new ConcurrentModificationException();
        return NULL;
    }
    if (mNextEntry == NULL) {
        // throw new NoSuchElementException();
        return NULL;
    }

    AutoPtr<HashtableEntry> entryToReturn = mNextEntry;
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mHost->mTable;
    AutoPtr<HashtableEntry> next = entryToReturn->mNext;
    while (next == NULL && mNextIndex < tab->GetLength()) {
        next = (*tab)[mNextIndex++];
    }
    mNextEntry = next;
    return mLastEntryReturned = entryToReturn;
}

AutoPtr<HashTable::HashtableEntry> HashTable::HashIterator::NextEntryNotFailFast()
{
    if (mNextEntry == NULL) {
        // throw new NoSuchElementException();
        return NULL;
    }

    AutoPtr<HashtableEntry> entryToReturn = mNextEntry;
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mHost->mTable;
    AutoPtr<HashtableEntry> next = entryToReturn->mNext;
    while (next == NULL && mNextIndex < tab->GetLength()) {
        next = (*tab)[mNextIndex++];
    }
    mNextEntry = next;
    return mLastEntryReturned = entryToReturn;
}

ECode HashTable::HashIterator::Remove()
{
    if (mLastEntryReturned == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mHost->mModCount != mExpectedModCount) {
        // throw new ConcurrentModificationException();
        return E_CONCURRENT_MODIFICATION_EXCEPTION;
    }
    AutoPtr<IInterface> res;
    mHost->Remove(mLastEntryReturned->mKey, (IInterface**)&res);
    mLastEntryReturned = NULL;
    mExpectedModCount = mHost->mModCount;
    return NOERROR;
}


//==========================================================
//       HashTable::KeyIterator
//==========================================================
CAR_INTERFACE_IMPL(HashTable::KeyIterator, IIterator);

HashTable::KeyIterator::KeyIterator(
    /* [in] */ HashTable* host)
    : HashIterator(host)
{
}

ECode HashTable::KeyIterator::Next(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<HashtableEntry> res = NextEntry();
    *outface = res->mKey;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode HashTable::KeyIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode HashTable::KeyIterator::Remove()
{
    return HashIterator::Remove();
}


//==========================================================
//       HashTable::ValueIterator
//==========================================================
CAR_INTERFACE_IMPL(HashTable::ValueIterator, IIterator);

HashTable::ValueIterator::ValueIterator(
    /* [in] */ HashTable* host)
    : HashIterator(host)
{
}

ECode HashTable::ValueIterator::Next(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<HashtableEntry> res = NextEntry();
    *outface = res->mValue;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode HashTable::ValueIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode HashTable::ValueIterator::Remove()
{
    return HashIterator::Remove();
}


//==========================================================
//       HashTable::EntryIterator
//==========================================================
CAR_INTERFACE_IMPL(HashTable::EntryIterator, IIterator);

HashTable::EntryIterator::EntryIterator(
    /* [in] */ HashTable* host)
    : HashIterator(host)
{
}

ECode HashTable::EntryIterator::Next(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<HashtableEntry> res = NextEntry();
    *outface = res->Probe(EIID_IInterface);
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode HashTable::EntryIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode HashTable::EntryIterator::Remove()
{
    return HashIterator::Remove();
}


//==========================================================
//       HashTable::KeyEnumeration
//==========================================================
CAR_INTERFACE_IMPL(HashTable::KeyEnumeration, IEnumeration);

HashTable::KeyEnumeration::KeyEnumeration(
    /* [in] */ HashTable* host)
    : HashIterator(host)
{
}

ECode HashTable::KeyEnumeration::HasMoreElements(
    /* [out] */ Boolean * value)
{
    return HasNext(value);
}

ECode HashTable::KeyEnumeration::NextElement(
    /* [out] */ IInterface ** inter)
{
    VALIDATE_NOT_NULL(inter)

    AutoPtr<HashtableEntry> res = NextEntryNotFailFast();
    *inter = res->mKey;
    INTERFACE_ADDREF(*inter)
    return NOERROR;
}


//==========================================================
//       HashTable::ValueEnumeration
//==========================================================
CAR_INTERFACE_IMPL(HashTable::ValueEnumeration, IEnumeration);

HashTable::ValueEnumeration::ValueEnumeration(
    /* [in] */ HashTable* host)
    : HashIterator(host)
{
}

ECode HashTable::ValueEnumeration::HasMoreElements(
    /* [out] */ Boolean * value)
{
    return HasNext(value);
}

ECode HashTable::ValueEnumeration::NextElement(
    /* [out] */ IInterface ** inter)
{
    VALIDATE_NOT_NULL(inter)

    AutoPtr<HashtableEntry> res = NextEntryNotFailFast();
    *inter = res->mValue;
    INTERFACE_ADDREF(*inter)
    return NOERROR;
}


//==========================================================
//       HashTable::_KeySet
//==========================================================
UInt32 HashTable::_KeySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HashTable::_KeySet::Release()
{
    return ElRefBase::Release();
}

PInterface HashTable::_KeySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode HashTable::_KeySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

HashTable::_KeySet::_KeySet(
    /* [in] */ HashTable* host)
{
    mHost = host;
}

ECode HashTable::_KeySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<IIterator> res = (IIterator*) new KeyIterator(mHost);
    *outiter = res;
    INTERFACE_ADDREF(*outiter)
    return NOERROR;
}

ECode HashTable::_KeySet::GetSize(
    /* [out] */ Int32* size)
{
    return mHost->GetSize(size);
}

ECode HashTable::_KeySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    return mHost->ContainsKey(o, value);
}

ECode HashTable::_KeySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(mHost->GetSelfLock());
    Int32 oldSize = mHost->mSize;
    AutoPtr<IInterface> res;
    mHost->Remove(o, (IInterface**)&res);
    *value = mHost->mSize != oldSize;
    return NOERROR;
}

ECode HashTable::_KeySet::Clear()
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return mHost->Clear();
}

ECode HashTable::_KeySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode HashTable::_KeySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode HashTable::_KeySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode HashTable::_KeySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::RemoveAll(collection, value);
}

ECode HashTable::_KeySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::RetainAll(collection, value);
}

ECode HashTable::_KeySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ContainsAll(collection, value);
}

ECode HashTable::_KeySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::Equals(object, value);
}

ECode HashTable::_KeySet::GetHashCode(
    /* [out] */ Int32* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::GetHashCode(value);
}

ECode HashTable::_KeySet::ToString(
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ToString(str);
}

ECode HashTable::_KeySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ToArray(array);
}

ECode HashTable::_KeySet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ToArrayEx(contents, outArray);
}


//==========================================================
//       HashTable::_Values
//==========================================================
UInt32 HashTable::_Values::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HashTable::_Values::Release()
{
    return ElRefBase::Release();
}

PInterface HashTable::_Values::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ICollection*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ICollection*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)this;
    }

    return NULL;
}

ECode HashTable::_Values::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ICollection*)this) {
        *pIID = EIID_ICollection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

HashTable::_Values::_Values(
    /* [in] */ HashTable* host)
{
    mHost = host;
}

ECode HashTable::_Values::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<IIterator> res = (IIterator*) new ValueIterator(mHost);
    *outiter = res;
    INTERFACE_ADDREF(*outiter)
    return NOERROR;
}

ECode HashTable::_Values::GetSize(
    /* [out] */ Int32* size)
{
    return mHost->GetSize(size);
}

ECode HashTable::_Values::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    return mHost->ContainsValue(o, value);
}

ECode HashTable::_Values::Clear()
{
    return mHost->Clear();
}

ECode HashTable::_Values::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    return AbstractCollection::Remove(o, value);
}

ECode HashTable::_Values::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::Add(object, modified);
}

ECode HashTable::_Values::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::AddAll(collection, modified);
}

ECode HashTable::_Values::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractCollection::IsEmpty(result);
}

ECode HashTable::_Values::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    return AbstractCollection::RemoveAll(collection, value);
}

ECode HashTable::_Values::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    return AbstractCollection::RetainAll(collection, value);
}

ECode HashTable::_Values::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode HashTable::_Values::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = (Int32)this;
    return NOERROR;
}

ECode HashTable::_Values::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractCollection::ContainsAll(collection, value);
}

ECode HashTable::_Values::ToString(
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractCollection::ToString(str);
}

ECode HashTable::_Values::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractCollection::ToArray(array);
}

ECode HashTable::_Values::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractCollection::ToArrayEx(contents, outArray);
}


//==========================================================
//       HashTable::_EntrySet
//==========================================================
UInt32 HashTable::_EntrySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HashTable::_EntrySet::Release()
{
    return ElRefBase::Release();
}

PInterface HashTable::_EntrySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode HashTable::_EntrySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

HashTable::_EntrySet::_EntrySet(
    /* [in] */ HashTable* host)
{
    mHost = host;
}

ECode HashTable::_EntrySet::GetIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<IIterator> res = (IIterator*) new EntryIterator(mHost);
    *outiter = res;
    INTERFACE_ADDREF(*outiter)
    return NOERROR;
}

ECode HashTable::_EntrySet::GetSize(
    /* [out] */ Int32* size)
{
    return mHost->GetSize(size);
}

ECode HashTable::_EntrySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (!(IMapEntry::Probe(o))) {
        *value = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    AutoPtr<IInterface> valueface;
    e->GetKey((IInterface**)&keyface);
    e->GetValue((IInterface**)&valueface);
    return mHost->ContainsMapping(keyface, valueface);
}

ECode HashTable::_EntrySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (!(IMapEntry::Probe(o))) {
        *value = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    AutoPtr<IInterface> valueface;
    e->GetKey((IInterface**)&keyface);
    e->GetValue((IInterface**)&valueface);
    return mHost->RemoveMapping(keyface, valueface);
}

ECode HashTable::_EntrySet::Clear()
{
    return mHost->Clear();
}

ECode HashTable::_EntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode HashTable::_EntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode HashTable::_EntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode HashTable::_EntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::RemoveAll(collection, value);
}

ECode HashTable::_EntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::RetainAll(collection, value);
}

ECode HashTable::_EntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ContainsAll(collection, value);
}

ECode HashTable::_EntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::Equals(object, value);
}

ECode HashTable::_EntrySet::GetHashCode(
    /* [out] */ Int32* value)
{
    return mHost->GetHashCode(value);
}

ECode HashTable::_EntrySet::ToString(
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ToString(str);
}

ECode HashTable::_EntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ToArray(array);
}

ECode HashTable::_EntrySet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    Mutex::Autolock lock(mHost->GetSelfLock());
    return AbstractSet::ToArrayEx(contents, outArray);
}

//==========================================================
//       HashTable
//==========================================================
const Int32 HashTable::MINIMUM_CAPACITY;

const Int32 HashTable::MAXIMUM_CAPACITY;

const AutoPtr<ArrayOf<HashTable::HashtableEntry*> > HashTable::EMPTY_TABLE = ArrayOf<HashTable::HashtableEntry*>::Alloc(MINIMUM_CAPACITY >> 1); // = new HashtableEntry[MINIMUM_CAPACITY >>> 1];

const Float HashTable::DEFAULT_LOAD_FACTOR;

const Int32 HashTable::CHARS_PER_ENTRY;

const Int64 HashTable::sSerialVersionUID;

const AutoPtr<ArrayOf<IObjectStreamField*> > HashTable::sSerialPersistentFields;// = {
    //     new ObjectStreamField("threshold", int.class),
    //     new ObjectStreamField("loadFactor", float.class),
    // };

HashTable::HashTable()
    : mSize(0)
    , mModCount(0)
    , mThreshold(0)
{
}

ECode HashTable::Init()
{
    mTable = EMPTY_TABLE;
    mThreshold = -1; // Forces first put invocation to replace EMPTY_TABLE
    return NOERROR;
}

ECode HashTable::Init(
    /* [in] */ Int32 capacity)
{
    if (capacity < 0) {
        // throw new IllegalArgumentException("Capacity: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (capacity == 0) {
        // @SuppressWarnings("unchecked")
        mTable = EMPTY_TABLE;
        mThreshold = -1; // Forces first put() to replace EMPTY_TABLE
        return NOERROR;
    }

    if (capacity < MINIMUM_CAPACITY) {
        capacity = MINIMUM_CAPACITY;
    }
    else if (capacity > MAXIMUM_CAPACITY) {
        capacity = MAXIMUM_CAPACITY;
    }
    else {
        capacity = RoundUpToPowerOfTwo(capacity);
    }
    MakeTable(capacity);
    return NOERROR;
}

ECode HashTable::Init(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    Init(capacity);

    AutoPtr<CFloat> fptr;
    FAIL_RETURN(CFloat::NewByFriend(loadFactor, (CFloat**)&fptr));
    Boolean isflag = FALSE;

    if (loadFactor <= 0 || (fptr->IsNaN(&isflag), isflag)) {
        // throw new IllegalArgumentException("Load factor: " + loadFactor);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    /*
     * Note that this implementation ignores loadFactor; it always uses
     * a load factor of 3/4. This simplifies the code and generally
     * improves performance.
     */
    return NOERROR;
}

ECode HashTable::Init(
    /* [in] */ IMap* map)
{
    Int32 sizelen = 0;
    map->GetSize(&sizelen);
    Init(CapacityForInitSize(sizelen));
    return ConstructorPutAll(map);
}

ECode HashTable::ConstructorPutAll(
    /* [in] */ IMap* map)
{
    AutoPtr<ArrayOf<IInterface*> > outarr;
    AutoPtr<ISet> outset;
    map->EntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        AutoPtr<IMapEntry> e = IMapEntry::Probe((*outarr)[i]);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        e->GetKey((IInterface**)&keyface);
        e->GetValue((IInterface**)&valueface);
        ConstructorPut(keyface, valueface);
    }
    return NOERROR;
}

Int32 HashTable::CapacityForInitSize(
    /* [in] */ Int32 size)
{
    Int32 result = (size >> 1) + size; // Multiply by 3/2 to allow for growth

    // boolean expr is equivalent to result >= 0 && result<MAXIMUM_CAPACITY
    return (result & ~(MAXIMUM_CAPACITY-1)) == 0 ? result : MAXIMUM_CAPACITY;
}

ECode HashTable::Clone(
    /* [out] */ IInterface** object)
{
    Mutex::Autolock lock(GetSelfLock());
    /*
     * This could be made more efficient. It unnecessarily hashes all of
     * the elements in the map.
     */
    AutoPtr<CHashTable> result;
    // try {
    // result = (Hashtable<K, V>) super.clone();
    FAIL_RETURN(CHashTable::NewByFriend((CHashTable**)&result));
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }

    // Restore clone to empty state, retaining our capacity and threshold
    result->MakeTable(mTable->GetLength());
    result->mSize = 0;
    result->mKeySet = NULL;
    result->mEntrySet = NULL;
    result->mValues = NULL;

    result->ConstructorPutAll((IMap*)this->Probe(EIID_IMap));
    *object = result->Probe(EIID_IInterface);
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode HashTable::IsEmpty(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(GetSelfLock());
    *value = mSize == 0;
    return NOERROR;
}

ECode HashTable::GetSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(GetSelfLock());
    *value = mSize;
    return NOERROR;
}

ECode HashTable::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    Mutex::Autolock lock(GetSelfLock());
    // Doug Lea's supplemental secondaryHash function (inlined)
    Int32 hash = ObjectUtils::GetHashCode(key);
    hash ^= ((UInt32)hash >> 20) ^ ((UInt32)hash >> 12);
    hash ^= ((UInt32)hash >> 7) ^ ((UInt32)hash >> 4);

    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    for (AutoPtr<HashtableEntry> e = (*tab)[hash & (tab->GetLength() - 1)];
            e != NULL; e = e->mNext) {
        AutoPtr<IInterface> eKey = e->mKey;
        if (eKey.Get() == key || (e->mHash == hash && ObjectUtils::Equals(key, eKey))) {
            *outface = e->mValue;
            INTERFACE_ADDREF(*outface)
            return NOERROR;
        }
    }
    *outface = NULL;
    return NOERROR;
}

ECode HashTable::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Mutex::Autolock lock(GetSelfLock());
    // Doug Lea's supplemental secondaryHash function (inlined)
    Int32 hash = ObjectUtils::GetHashCode(key);
    hash ^= ((UInt32)hash >> 20) ^ ((UInt32)hash >> 12);
    hash ^= ((UInt32)hash >> 7) ^ ((UInt32)hash >> 4);

    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    for (AutoPtr<HashtableEntry> e = (*tab)[hash & (tab->GetLength() - 1)];
            e != NULL; e = e->mNext) {
        AutoPtr<IInterface> eKey = e->mKey;
        if (eKey.Get() == key || (e->mHash == hash && ObjectUtils::Equals(key, eKey))) {
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode HashTable::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (value == NULL) {
        // throw new NullPointerException("value == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    Int32 len = tab->GetLength();

    for (Int32 i = 0; i < len; i++) {
        for (AutoPtr<HashtableEntry> e = (*tab)[i]; e != NULL; e = e->mNext) {
            if (value == e->mValue) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }

    return NOERROR;
}

ECode HashTable::Contains(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return ContainsValue(value, result);
}

ECode HashTable::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** outface)
{
    if (key == NULL || value == NULL) {
        if (outface) {
            *outface = NULL;
        }
        // throw new NullPointerException("key or value is null");
        return E_NULL_POINTER_EXCEPTION;
    }

    Mutex::Autolock lock(GetSelfLock());
    Int32 keyhash = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keyhash);
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    AutoPtr<HashtableEntry> first = (*tab)[index];
    for (AutoPtr<HashtableEntry> e = first; e != NULL; e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            if (outface) {
                *outface = e->mValue;;
                INTERFACE_ADDREF(*outface)
            }

            e->mValue = value;
            return NOERROR;
        }
    }

    // No entry for key is present; create one
    mModCount++;
    if (mSize++ > mThreshold) {
        Rehash();  // Does nothing!!
        tab = DoubleCapacity();
        index = hash & (tab->GetLength() - 1);
        first = (*tab)[index];
    }
    AutoPtr<HashtableEntry> res = new HashtableEntry(key, value, hash, first);
    tab->Set(index, res);
    if (outface) {
        *outface = NULL;
    }
    return NOERROR;
}

ECode HashTable::ConstructorPut(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    if (key == NULL) {
        // throw new NullPointerException("key == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    else if (value == NULL) {
        // throw new NullPointerException("value == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 keycode = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keycode);
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    AutoPtr<HashtableEntry> first = (*tab)[index];
    for (AutoPtr<HashtableEntry> e = first; e != NULL; e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            e->mValue = value;
            return NOERROR;
        }
    }

    // No entry for key is present; create one
    AutoPtr<HashtableEntry> res = new HashtableEntry(key, value, hash, first);
    tab->Set(index, res);
    mSize++;
    return NOERROR;
}

ECode HashTable::PutAll(
    /* [in] */ IMap* map)
{
    Mutex::Autolock lock(GetSelfLock());
    Int32 sizelen = 0;
    map->GetSize(&sizelen);
    EnsureCapacity(sizelen);
    AutoPtr<ArrayOf<IInterface*> > outarr;
    AutoPtr<ISet> outset;
    map->EntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        AutoPtr<IMapEntry> e = IMapEntry::Probe((*outarr)[i]);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        e->GetKey((IInterface**)&keyface);
        e->GetValue((IInterface**)&valueface);
        AutoPtr<IInterface> outface;
        Put(keyface, valueface, (IInterface**)&outface);
    }
    return NOERROR;
}

ECode HashTable::EnsureCapacity(
    /* [in] */ Int32 numMappings)
{
    Int64 newCapacity = RoundUpToPowerOfTwo(CapacityForInitSize(numMappings));
    AutoPtr<ArrayOf<HashtableEntry*> > oldTable = mTable;
    Int32 oldCapacity = oldTable->GetLength();
    if (newCapacity <= oldCapacity) {
        return NOERROR;
    }

    Rehash();  // Does nothing!!

    if (newCapacity == oldCapacity * 2) {
        DoubleCapacity();
        return NOERROR;
    }

    // We're growing by at least 4x, rehash in the obvious way
    AutoPtr<ArrayOf<HashtableEntry*> > newTable = MakeTable(newCapacity);
    if (mSize != 0) {
        Int32 newMask = newCapacity - 1;
        for (Int32 i = 0; i < oldCapacity; i++) {
            for (AutoPtr<HashtableEntry> e = (*oldTable)[i]; e != NULL;) {
                AutoPtr<HashtableEntry> oldNext = e->mNext;
                Int32 newIndex = e->mHash & newMask;
                AutoPtr<HashtableEntry> newNext = (*newTable)[newIndex];
                newTable->Set(newIndex, e);
                e->mNext = newNext;
                e = oldNext;
            }
        }
    }
    return NOERROR;
}

ECode HashTable::Rehash()
{
    /*
     * This method has no testable semantics, other than that it gets
     * called from time to time.
     */
    return E_NOT_IMPLEMENTED;
}

AutoPtr<ArrayOf<HashTable::HashtableEntry*> > HashTable::MakeTable(
    /* [in] */ Int32 newCapacity)
{
    // @SuppressWarnings("unchecked")
    AutoPtr<ArrayOf<HashtableEntry*> > newTable = ArrayOf<HashtableEntry*>::Alloc(newCapacity);
    mTable = newTable;
    mThreshold = ((UInt32)newCapacity >> 1) + ((UInt32)newCapacity >> 2); // 3/4 capacity
    return newTable;
}

AutoPtr<ArrayOf<HashTable::HashtableEntry*> > HashTable::DoubleCapacity()
{
    AutoPtr<ArrayOf<HashtableEntry*> > oldTable = mTable;
    Int32 oldCapacity = oldTable->GetLength();
    if (oldCapacity == MAXIMUM_CAPACITY) {
        return oldTable;
    }
    Int32 newCapacity = oldCapacity * 2;
    AutoPtr<ArrayOf<HashtableEntry*> > newTable = MakeTable(newCapacity);
    if (mSize == 0) {
        return newTable;
    }

    for (Int32 j = 0; j < oldCapacity; j++) {
        /*
         * Rehash the bucket using the minimum number of field writes.
         * This is the most subtle and delicate code in the class.
         */
        AutoPtr<HashtableEntry> e = (*oldTable)[j];
        if (e == NULL) {
            continue;
        }
        Int32 highBit = e->mHash & oldCapacity;
        AutoPtr<HashtableEntry> broken;
        newTable->Set(j | highBit, e);
        for (AutoPtr<HashtableEntry> n = e->mNext; n != NULL; e = n, n = n->mNext) {
            Int32 nextHighBit = n->mHash & oldCapacity;
            if (nextHighBit != highBit) {
                if (broken == NULL)
                    newTable->Set(j | nextHighBit, n);
                else
                    broken->mNext = n;
                broken = e;
                highBit = nextHighBit;
            }
        }
        if (broken != NULL)
            broken->mNext = NULL;
    }
    return newTable;
}

ECode HashTable::Remove(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    Mutex::Autolock lock(GetSelfLock());
    Int32 keyhash = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keyhash);
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    AutoPtr<HashtableEntry> prev;
    for ( AutoPtr<HashtableEntry> e = (*tab)[index]; e != NULL; prev = e, e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(key, e->mKey)) {
            if (prev == NULL) {
                tab->Set(index, e->mNext);
            }
            else {
                prev->mNext = e->mNext;
            }
            mModCount++;
            mSize--;
            *outface = e->mValue;
            INTERFACE_ADDREF(*outface)
            return NOERROR;
        }
    }
    *outface = NULL;
    return NOERROR;
}

ECode HashTable::Clear()
{
    Mutex::Autolock lock(GetSelfLock());
    if (mSize != 0) {
        // Arrays.fill(table, null);
        for (Int32 i = 0; i < mTable->GetLength(); ++i) {
            mTable->Set(i, NULL);
        }
        mModCount++;
        mSize = 0;
    }
    return NOERROR;
}

ECode HashTable::KeySet(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<ISet> ks = mKeySet;
    *keySet = ks != NULL ? ks : (mKeySet = (ISet*) new _KeySet(this));
    INTERFACE_ADDREF(*keySet)
    return NOERROR;
}

ECode HashTable::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<ICollection> vs = mValues;
    *value = vs != NULL ? vs : (mValues = (ICollection*) new _Values(this));
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode HashTable::EntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)

    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<ISet> es = mEntrySet;
    *entries = (es != NULL) ? es : (mEntrySet = (ISet*) new _EntrySet(this));
    INTERFACE_ADDREF(*entries)
    return NOERROR;
}

ECode HashTable::Keys(
    /* [out] */ IEnumeration** enm)
{
    VALIDATE_NOT_NULL(enm)

    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<IEnumeration> res = (IEnumeration*) new KeyEnumeration(this);
    *enm = res;
    INTERFACE_ADDREF(*enm)
    return NOERROR;
}

ECode HashTable::Elements(
    /* [out] */ IEnumeration** enm)
{
    VALIDATE_NOT_NULL(enm)

    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<IEnumeration> res = (IEnumeration*) new ValueEnumeration(this);
    *enm = res;
    INTERFACE_ADDREF(*enm)
    return NOERROR;
}

Boolean HashTable::ContainsMapping(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    Mutex::Autolock lock(GetSelfLock());
    Int32 keycode = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keycode);
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    for (AutoPtr<HashtableEntry> e = (*tab)[index]; e != NULL; e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(e->mKey, key)) {
            return ObjectUtils::Equals(e->mValue, value);
        }
    }
    return FALSE; // No entry for key
}

Boolean HashTable::RemoveMapping(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    Mutex::Autolock lock(GetSelfLock());
    Int32 keycode = ObjectUtils::GetHashCode(key);
    Int32 hash = SecondaryHash(keycode);
    AutoPtr<ArrayOf<HashtableEntry*> > tab = mTable;
    Int32 index = hash & (tab->GetLength() - 1);
    AutoPtr<HashtableEntry> prev;
    for (AutoPtr<HashtableEntry> e = (*tab)[index]; e != NULL; prev = e, e = e->mNext) {
        if (e->mHash == hash && ObjectUtils::Equals(e->mKey, key)) {
            if (!(e->mValue.Get() == value)) {
                return FALSE;  // Map has wrong value for key
            }
            if (prev == NULL) {
                tab->Set(index, e->mNext);
            }
            else {
                prev->mNext = e->mNext;
            }
            mModCount++;
            mSize--;
            return TRUE;
        }
    }
    return FALSE; // No entry for key
}

ECode HashTable::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<IMap> res = (IMap*) object->Probe(EIID_IMap);
    AutoPtr<ISet> outarr;
    AutoPtr<ISet> outarr2;
    EntrySet((ISet**)&outarr);
    Boolean isflag = FALSE;
    *result = (res != NULL) && (outarr->Equals((res->EntrySet((ISet**)&outarr2), outarr2), &isflag), isflag);
    return NOERROR;
}

ECode HashTable::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    Mutex::Autolock lock(GetSelfLock());
    Int32 result = 0;
    AutoPtr<ArrayOf<IInterface*> > outarr;
    AutoPtr<ISet> outset;
    EntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        AutoPtr<IMapEntry> e = IMapEntry::Probe((*outarr)[i]);
        AutoPtr<IInterface> key;
        e->GetKey((IInterface**)&key);
        AutoPtr<IInterface> value;
        e->GetValue((IInterface**)&value);
        if (key.Get() == this->Probe(EIID_IInterface) || value.Get() == this->Probe(EIID_IInterface)) {
            continue;
        }
        Int32 keyhash = ObjectUtils::GetHashCode(key);
        Int32 valuehash = ObjectUtils::GetHashCode(value);
        result += (key != NULL ? keyhash : 0)
                ^ (value != NULL ? valuehash : 0);
    }
    *hashCode = result;
    return NOERROR;
}

ECode HashTable::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    Mutex::Autolock lock(GetSelfLock());
    StringBuilder result;
    result.AppendChar('{');
    AutoPtr<ISet> outarr;
    EntrySet((ISet**)&outarr);
    AutoPtr<IIterator> i;
    outarr->GetIterator((IIterator**)&i);
    Boolean hasMore = FALSE;
    i->HasNext(&hasMore);
    while (hasMore) {
        AutoPtr<IInterface> outface;
        i->Next((IInterface**)&outface);
        AutoPtr<IMapEntry> entry = IMapEntry::Probe(outface);

        AutoPtr<IInterface> key;
        entry->GetKey((IInterface**)&key);
        String keystr;
        result.AppendString(key.Get() == this->Probe(EIID_IInterface)
            ? String("(this Map)") : ObjectUtils::ToString(key));

        result.AppendChar('=');

        AutoPtr<IInterface> value;
        entry->GetValue((IInterface**)&value);
        String valuestr;
        result.AppendString(value.Get() == this->Probe(EIID_IInterface)
            ? String("(this Map)") : ObjectUtils::ToString(value));

        if (i->HasNext(&hasMore), hasMore) {
            result.AppendCStr(", ");
        }
    }

    result.AppendChar('}');
    return result.ToString(str);
}

Int32 HashTable::SecondaryHash(
    /* [in] */ Int32 h)
{
    // Doug Lea's supplemental hash function
    h ^= ((UInt32)h >> 20) ^ ((UInt32)h >> 12);
    return h ^ ((UInt32)h >> 7) ^ ((UInt32)h >> 4);
}

Int32 HashTable::RoundUpToPowerOfTwo(
    /* [in] */ Int32 i)
{
    i--; // If input is a power of two, shift its high-order bit right

    // "Smear" the high-order bit all the way to the right
    i |= (UInt32)i >>  1;
    i |= (UInt32)i >>  2;
    i |= (UInt32)i >>  4;
    i |= (UInt32)i >>  8;
    i |= (UInt32)i >> 16;

    return i + 1;
}

ECode HashTable::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(GetSelfLock());
    // Emulate loadFactor field for other implementations to read
    AutoPtr<IObjectOutputStreamPutField> fields;
    stream->PutFields((IObjectOutputStreamPutField**)&fields);
    fields->PutInt32(String("threshold"),  (Int32) (DEFAULT_LOAD_FACTOR * mTable->GetLength()));
    fields->PutFloat(String("loadFactor"), DEFAULT_LOAD_FACTOR);
    stream->WriteFields();

    stream->Write(mTable->GetLength()); // Capacity
    stream->Write(mSize);
    AutoPtr<ArrayOf<IInterface*> > outarr;
    AutoPtr<ISet> outset;
    EntrySet((ISet**)&outset);
    outset->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        AutoPtr<IMapEntry> e = IMapEntry::Probe((*outarr)[i]);
        assert(0 && "TODO");
        // stream.writeObject(e.getKey());
        // stream.writeObject(e.getValue());
    }
    return NOERROR;
}

ECode HashTable::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    stream->DefaultReadObject();
    Int32 capacity = 0;
    stream->Read(&capacity);
    if (capacity < 0) {
        // throw new InvalidObjectException("Capacity: " + capacity);
        return E_INVALID_OBJECT_EXCEPTION;
    }
    if (capacity < MINIMUM_CAPACITY) {
        capacity = MINIMUM_CAPACITY;
    }
    else if (capacity > MAXIMUM_CAPACITY) {
        capacity = MAXIMUM_CAPACITY;
    }
    else {
        capacity = RoundUpToPowerOfTwo(capacity);
    }
    MakeTable(capacity);

    Int32 size = 0;
    stream->Read(&size);
    if (size < 0) {
        // throw new InvalidObjectException("Size: " + size);
        return E_INVALID_OBJECT_EXCEPTION;
    }

    for (Int32 i = 0; i < size; i++) {
        assert(0 && "TODO");
        // @SuppressWarnings("unchecked") K key = (K) stream.readObject();
        // @SuppressWarnings("unchecked") V val = (V) stream.readObject();
        // constructorPut(key, val);
    }
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
