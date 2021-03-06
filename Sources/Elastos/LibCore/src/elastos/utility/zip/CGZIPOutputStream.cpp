
#include "cmdef.h"
#include "CGZIPOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CGZIPOutputStream::CGZIPOutputStream()
{
    CCRC32::NewByFriend((CCRC32**)&mCrc);
}

ECode CGZIPOutputStream::Finish()
{
    FAIL_RETURN(DeflaterOutputStream::Finish());
    Int64 checksum;
    mCrc->GetValue(&checksum);
    WriteInt64(checksum);
    WriteInt64(mCrc->mTbytes);
    return NOERROR;
}

ECode CGZIPOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    FAIL_RETURN(DeflaterOutputStream::WriteBytesEx(buffer, off, nbytes));
    return mCrc->UpdateEx2(buffer, off, nbytes);
}

Int64 CGZIPOutputStream::WriteInt64(
    /* [in] */ Int64 i)
{
    // Write out the long value as an unsigned int
    Int32 ui = (Int32)i;
    mOut->Write(ui & 0xFF);
    mOut->Write((ui >> 8) & 0xFF);
    mOut->Write((ui >> 16) & 0xFF);
    mOut->Write((ui >> 24) & 0xFF);
    return i;
}

Int32 CGZIPOutputStream::WriteInt32(
    /* [in] */ Int32 i)
{
    mOut->Write(i & 0xFF);
    mOut->Write((i >> 8) & 0xFF);
    return i;
}

ECode CGZIPOutputStream::Close()
{
    return DeflaterOutputStream::Close();
}

ECode CGZIPOutputStream::Flush()
{
    return DeflaterOutputStream::Flush();
}

ECode CGZIPOutputStream::Write(
    /* [in] */ Int32 val)
{
    return DeflaterOutputStream::Write(val);
}

ECode CGZIPOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return DeflaterOutputStream::WriteBytes(buffer);
}

ECode CGZIPOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    return DeflaterOutputStream::CheckError(hasError);
}

ECode CGZIPOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return constructor(os, BUF_SIZE);
}

ECode CGZIPOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ Int32 size)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend(IDeflater::DEFAULT_COMPRESSION, TRUE, (CDeflater**)&deflater);
    FAIL_RETURN(DeflaterOutputStream::Init(os, deflater.Get(), size));
    WriteInt32(IGZIPInputStream::MAGIC);
    FAIL_RETURN(mOut->Write(IDeflater::DEFLATED));
    FAIL_RETURN(mOut->Write(0)); // flags
    WriteInt64(0); // mod time
    FAIL_RETURN(mOut->Write(0)); // extra flags
    return mOut->Write(0); // operating system
}

ECode CGZIPOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = DeflaterOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

PInterface CGZIPOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CGZIPOutputStream::Probe(riid);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
