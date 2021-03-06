#include "CUUID.h"
#include "cmdef.h"
#include <elastos/StringUtils.h>
#include <elastos/StringBuilder.h>
#include "CMemory.h"

using Elastos::IO::IMemory;
using Elastos::IO::CMemory;
using Elastos::IO::ByteOrder_BIG_ENDIAN;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Utility {

CUUID::CUUID()
    : mMostSigBits(0)
    , mLeastSigBits(0)
    , mVariant(0)
    , mVersion(0)
    , mTimestamp(0)
    , mClockSequence(0)
    , mNode(0)
    , mHash(0)
{
}

ECode CUUID::constructor(
    /* [in] */ Int64 mostSigBits,
    /* [in] */ Int64 leastSigBits)
{
    mMostSigBits = mostSigBits;
    mLeastSigBits = leastSigBits;
    return Init();
}

ECode CUUID::Init()
{
    // setup hash field
    Int32 msbHash = (Int32) (mMostSigBits ^ ((UInt64)mMostSigBits >> 32));
    Int32 lsbHash = (Int32) (mLeastSigBits ^ ((UInt64)mLeastSigBits >> 32));
    mHash = msbHash ^ lsbHash;

    // setup variant field
    if ((mLeastSigBits & 0x8000000000000000LL) == 0) {
        // MSB0 not set, NCS backwards compatibility variant
        mVariant = 0;
    }
    else if ((mLeastSigBits & 0x4000000000000000LL) != 0) {
        // MSB1 set, either MS reserved or future reserved
        mVariant = (Int32) (UInt64(mLeastSigBits & 0xE000000000000000LL) >> 61);
    }
    else {
        // MSB1 not set, RFC 4122 variant
        mVariant = 2;
    }

    // setup version field
    mVersion = (Int32) ((UInt64)(mMostSigBits & 0x000000000000F000) >> 12);

    if (mVariant != 2 && mVersion != 1) {
        return NOERROR;
    }

    // setup timestamp field
    Int64 timeLow = UInt64(mMostSigBits & 0xFFFFFFFF00000000LL) >> 32;
    Int64 timeMid = (mMostSigBits & 0x00000000FFFF0000LL) << 16;
    Int64 timeHigh = (mMostSigBits & 0x0000000000000FFFLL) << 48;
    mTimestamp = timeLow | timeMid | timeHigh;

    // setup clock sequence field
    mClockSequence = (Int32) (UInt64(mLeastSigBits & 0x3FFF000000000000LL) >> 48);

    // setup node field
    mNode = (mLeastSigBits & 0x0000FFFFFFFFFFFFLL);

    return NOERROR;
}

ECode CUUID::RandomUUID(
    /* [out] */ IUUID** uuid)
{
    VALIDATE_NOT_NULL(uuid);
    *uuid = NULL;

    Byte data[16];
    // // lock on the class to protect lazy init
    // synchronized (UUID.class) {
    //     if (rng == null) {
    //         rng = new SecureRandom();
    //     }
    // }
    // rng.nextBytes(data);
    // return MakeUuid(data, 4);
    // assert(0 && "TODO");
    return NOERROR;
}

ECode CUUID::NameUUIDFromBytes(
    /* [in] */ const ArrayOf<Byte>& name,
    /* [out] */ IUUID** uuid)
{
    VALIDATE_NOT_NULL(uuid)
    *uuid = NULL;

    if (name.GetLength() == 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new NullPointerException("name == null");
    }
    // try {
    //     MessageDigest md = MessageDigest.getInstance("MD5");
    //     return MakeUuid(md.digest(name), 3);
    // } catch (NoSuchAlgorithmException e) {
    //     throw new AssertionError(e);
    // }
    assert(0 && "TODO");
    return NOERROR;
}

AutoPtr<IUUID> CUUID::MakeUuid(
    /* [in] */ const ArrayOf<Byte>& hash,
    /* [in] */ Int32 version)
{
    AutoPtr<IMemory> memory;
    CMemory::AcquireSingleton((IMemory**)&memory);
    Int64 msb, lsb;
    memory->PeekInt64(hash, 0, ByteOrder_BIG_ENDIAN, &msb);
    memory->PeekInt64(hash, 8, ByteOrder_BIG_ENDIAN, &lsb);
    // Set the version field.
    msb &= ~(0xfLL << 12);
    msb |= ((Int64) version) << 12;
    // Set the variant field to 2. Note that the variant field is variable-width,
    // so supporting other variants is not just a matter of changing the constant 2 below!
    lsb &= ~(0x3LL << 62);
    lsb |= 2LL << 62;

    AutoPtr<CUUID> uuid;
    CUUID::NewByFriend(msb, lsb, (CUUID**)&uuid);
    AutoPtr<IUUID> id = (IUUID*)uuid.Get();
    return id;
}

ECode CUUID::FromString(
    /* [in] */ const String& name,
    /* [out] */ IUUID** uuid)
{
    VALIDATE_NOT_NULL(uuid);
    *uuid = NULL;
    if (name.IsNull())
        return E_NULL_POINTER_EXCEPTION;

    const Int32 POSITION_LENGTH = 5;
    Int32 position[POSITION_LENGTH];
    Int32 lastPosition = 1;
    Int32 startPosition = 0;

    Int32 i = 0;
    for (; i < POSITION_LENGTH && lastPosition > 0; i++) {
        position[i] = name.IndexOf('-', startPosition);
        lastPosition = position[i];
        startPosition = position[i] + 1;
    }

    // should have and only can have four "-" in UUID
    if (i != POSITION_LENGTH || lastPosition != -1) {
        // throw new IllegalArgumentException("Invalid UUID: " + name);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 m1, m2, m3;
    String s1 = name.Substring(0, position[0]);
    String s2 = name.Substring(position[0] + 1, position[1]);
    String s3 = name.Substring(position[1] + 1, position[2]);
    FAIL_RETURN(StringUtils::ParseInt64(s1, 16, &m1));
    FAIL_RETURN(StringUtils::ParseInt64(s2, 16, &m2));
    FAIL_RETURN(StringUtils::ParseInt64(s3, 16, &m3));

    String ls1 = name.Substring(position[2] + 1, position[3]);
    String ls2 = name.Substring(position[3] + 1);
    Int64 lsb1, lsb2;
    FAIL_RETURN(StringUtils::ParseInt64(ls1, 16, &lsb1));
    FAIL_RETURN(StringUtils::ParseInt64(ls2, 16, &lsb2));

    Int64 msb = (m1 << 32) | (m2 << 16) | m3;
    Int64 lsb = (lsb1 << 48) | lsb2;
    AutoPtr<CUUID> outuid;
    FAIL_RETURN(CUUID::NewByFriend(msb, lsb, (CUUID**)&outuid));
    *uuid = (IUUID*) outuid.Get();
    INTERFACE_ADDREF(*uuid)
    return NOERROR;
}

ECode CUUID::GetLeastSignificantBits(
    /* [out] */ Int64* leastSigBits)
{
    VALIDATE_NOT_NULL(leastSigBits)
    *leastSigBits = mLeastSigBits;
    return NOERROR;
}

ECode CUUID::GetMostSignificantBits(
    /* [out] */ Int64* mostSigBits)
{
    VALIDATE_NOT_NULL(mostSigBits)
    *mostSigBits = mMostSigBits;
    return NOERROR;
}

ECode CUUID::Version(
    /* [out] */ Int32* version)
{
    VALIDATE_NOT_NULL(version)
    *version = mVersion;
    return NOERROR;
}

ECode CUUID::Variant(
    /* [out] */ Int32* variant)
{
    VALIDATE_NOT_NULL(variant)
    *variant = mVariant;
    return NOERROR;
}

ECode CUUID::Timestamp(
    /* [out] */ Int64* timeStamp)
{
    VALIDATE_NOT_NULL(timeStamp)
    *timeStamp = 0;

    if (mVersion != 1) {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    *timeStamp = mTimestamp;
    return NOERROR;
}

ECode CUUID::ClockSequence(
    /* [out] */ Int32* clockSequence)
{
    VALIDATE_NOT_NULL(clockSequence)
    *clockSequence = 0;

    if (mVersion != 1) {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    *clockSequence = mClockSequence;
    return NOERROR;
}

ECode CUUID::Node(
    /* [out] */ Int64* node)
{
    VALIDATE_NOT_NULL(node)
    *node = 0;

    if (mVersion != 1) {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    *node = mNode;
    return NOERROR;
}

ECode CUUID::CompareTo(
    /* [in] */ IUUID* other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (other == NULL) {
        *result = -1;
        return NOERROR;
    }

    *result = 0;
    if (other == (IUUID*)this) {
        return NOERROR;
    }

    Int64 om, ol;
    other->GetMostSignificantBits(&om);
    other->GetLeastSignificantBits(&ol);

    if (mMostSigBits != om) {
        *result = mMostSigBits < om ? -1 : 1;
        return NOERROR;
    }

    if (mLeastSigBits != ol) {
        *result = mLeastSigBits < ol ? -1 : 1;
        return NOERROR;
    }

    return NOERROR;
}

ECode CUUID::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (other == NULL) {
        return NOERROR;
    }

    if (IUUID::Probe(other) == NULL)
        return NOERROR;

    if (IUUID::Probe(other) == (IUUID*)this) {
        *result = TRUE;
        return NOERROR;
    }

    Int64 om, ol;
    IUUID::Probe(other)->GetMostSignificantBits(&om);
    IUUID::Probe(other)->GetLeastSignificantBits(&ol);
    *result = (mLeastSigBits == ol) && (mMostSigBits == om);
    return NOERROR;
}

ECode CUUID::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash)
    *hash = mHash;
    return NOERROR;
}

ECode CUUID::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder builder(36);

    String msbStr = StringUtils::Int64ToHexString(mMostSigBits);
    if (msbStr.GetLength() < 16) {
        Int32 diff = 16 - msbStr.GetLength();
        for (Int32 i = 0; i < diff; i++) {
            builder.AppendChar('0');
        }
    }
    builder.AppendString(msbStr);
    builder.InsertChar(8, '-');
    builder.InsertChar(13, '-');
    builder.AppendChar('-');
    String lsbStr = StringUtils::Int64ToHexString(mLeastSigBits);
    if (lsbStr.GetLength() < 16) {
        Int32 diff = 16 - lsbStr.GetLength();
        for (Int32 i = 0; i < diff; i++) {
            builder.AppendChar('0');
        }
    }
    builder.AppendString(lsbStr);
    builder.InsertChar(23, '-');
    *str = builder.ToString();
    return NOERROR;
}

ECode CUUID::ReadObject(
    /* [in] */ IObjectInputStream* in)
{
    VALIDATE_NOT_NULL(in);
    // read in non-transient fields
    in->DefaultReadObject();
    // setup transient fields
    return Init();
}

} // namespace Utility
} // namespace Elastos
