
#include "CCopyOnWriteArraySet.h"
#include "CCopyOnWriteArrayList.h"
#include <elastos/ObjectUtils.h>

using Elastos::Core::ObjectUtils;

namespace Elastos {
namespace Utility {
namespace Concurrent {

ECode CCopyOnWriteArraySet::constructor()
{
    return CCopyOnWriteArrayList::New((ICopyOnWriteArrayList**)&mAl);
}

ECode CCopyOnWriteArraySet::constructor(
    /* [in] */ ICollection* c)
{
    FAIL_RETURN(CCopyOnWriteArrayList::New((ICopyOnWriteArrayList**)&mAl));
    Int32 value = 0;
    return mAl->AddAllAbsent(c, &value);
}

PInterface CCopyOnWriteArraySet::Probe(
    /* [in] */ REIID riid)
{
    return _CCopyOnWriteArraySet::Probe(riid);
}

ECode CCopyOnWriteArraySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return mAl->AddIfAbsent(object, modified);
}

ECode CCopyOnWriteArraySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)

    Int32 value = 0;
    mAl->AddAllAbsent(collection, &value);
    *modified = value > 0;
    return NOERROR;
}

ECode CCopyOnWriteArraySet::Clear()
{
    return mAl->Clear();
}

ECode CCopyOnWriteArraySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return mAl->Contains(object, result);
}

ECode CCopyOnWriteArraySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return mAl->ContainsAll(collection, result);
}

ECode CCopyOnWriteArraySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (object == this->Probe(EIID_IInterface)) {
        *result = TRUE;
        return NOERROR;
    }
    if (!ISet::Probe(object)) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<ISet> set = ISet::Probe(object);
    AutoPtr<IIterator> it;
    set->GetIterator((IIterator**)&it);

    // Uses O(n^2) algorithm that is only appropriate
    // for small sets, which CopyOnWriteArraySets should be.

    //  Use a single snapshot of underlying array
    AutoPtr< ArrayOf<IInterface*> > elements;
    mAl->GetArray((ArrayOf<IInterface*>**)&elements);
    Int32 len = elements->GetLength();
    // Mark matched elements to avoid re-checking
    AutoPtr< ArrayOf<Boolean> > matched = ArrayOf<Boolean>::Alloc(len);
    for (Int32 i = 0; i < len; ++i) {
        (*matched)[i] = FALSE;
    }
    Int32 k = 0;
    Boolean hasNext = FALSE;
outer:
    while (it->HasNext(&hasNext), hasNext) {
        if (++k > len) {
            *result = FALSE;
            return NOERROR;
        }
        AutoPtr<IInterface> x;
        it->Next((IInterface**)&x);
        for (Int32 i = 0; i < len; ++i) {
            if (!(*matched)[i] && Eq(x, (*elements)[i])) {
                (*matched)[i] = TRUE;
                goto outer;
            }
        }
        *result = FALSE;
        return NOERROR;
    }
    *result = k == len;
    return NOERROR;
}

ECode CCopyOnWriteArraySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CCopyOnWriteArraySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return mAl->IsEmpty(result);
}

ECode CCopyOnWriteArraySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return mAl->Remove(object, modified);
}

ECode CCopyOnWriteArraySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return mAl->RemoveAll(collection, modified);
}

ECode CCopyOnWriteArraySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return mAl->RetainAll(collection, modified);
}

ECode CCopyOnWriteArraySet::GetSize(
    /* [out] */ Int32* size)
{
    return mAl->GetSize(size);
}

ECode CCopyOnWriteArraySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return mAl->ToArray(array);
}

ECode CCopyOnWriteArraySet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return mAl->ToArrayEx(inArray, outArray);
}

ECode CCopyOnWriteArraySet::GetIterator(
    /* [out] */ IIterator** it)
{
    return mAl->GetIterator(it);
}

Boolean CCopyOnWriteArraySet::Eq(
    /* [in] */ IInterface* o1,
    /* [in] */ IInterface* o2)
{
    return (o1 == NULL) ? o2 == NULL : ObjectUtils::Equals(o1, o2);
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
