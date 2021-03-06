
#include "cmdef.h"
#include "ZipFile.h"
#include "CInflater.h"
#include <elastos/Math.h>
#include <cutils/log.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::Math;
using Elastos::Core::CObjectContainer;
using Elastos::IO::EIID_IInputStream;
using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::EIID_IFilterInputStream;
using Elastos::IO::IDataInputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::IDataInput;
using Elastos::IO::IHeapBufferIterator;
using Elastos::IO::CHeapBufferIterator;
using Elastos::IO::ByteOrder_LITTLE_ENDIAN;
using Elastos::IO::EIID_IDataInput;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CRandomAccessFile;
using Elastos::IO::CFile;

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Utility {
namespace Zip {

extern "C" const InterfaceID EIID_ZipFileRAFStream =
    { 0x83f2304, 0xfc73, 0x4727, { 0x86, 0x6a, 0x81, 0xd5, 0xeb, 0x82, 0xf3, 0x43 } };

const Int32 ZipFile::GPBF_DATA_DESCRIPTOR_FLAG = 1 << 3;
const Int32 ZipFile::GPBF_UTF8_FLAG = 1 << 11;

ZipFile::RAFStream::RAFStream(
    /* [in] */ IRandomAccessFile* raf,
    /* [in] */ Int64 pos)
    : mSharedRaf(raf)
    , mOffset(pos)
    , mLength(0)
{
    raf->GetLength(&mLength);
}

PInterface ZipFile::RAFStream::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IInputStream*)this;
    }
    else if (riid == EIID_IInputStream) {
        return (IInputStream*)this;
    }
    else if (riid == EIID_ZipFileRAFStream) {
        return this;
    }
    return NULL;
}

UInt32 ZipFile::RAFStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ZipFile::RAFStream::Release()
{
    return ElRefBase::Release();
}

ECode ZipFile::RAFStream::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ZipFile::RAFStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = (mOffset < mLength ? 1 : 0);
    return NOERROR;
}

ECode ZipFile::RAFStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    return streams->ReadSingleByte(THIS_PROBE(IInputStream), value);
}

ECode ZipFile::RAFStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    Mutex::Autolock lock(mSharedRafLock);

    mSharedRaf->Seek(mOffset);
    if (length > mLength - mOffset) {
        length = (Int32)(mLength - mOffset);
    }
    FAIL_RETURN(mSharedRaf->ReadBytesEx(buffer, offset, length, number));
    if (*number > 0) {
        mOffset += *number;
    }
    else *number = -1;

    return NOERROR;
}

ECode ZipFile::RAFStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    if (byteCount > mLength - mOffset) {
        byteCount = mLength - mOffset;
    }
    mOffset += byteCount;
    *number = byteCount;
    return NOERROR;
}

ECode ZipFile::RAFStream::Close()
{
    return InputStream::Close();
}

ECode ZipFile::RAFStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode ZipFile::RAFStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    *supported = FALSE;
    return NOERROR;
}

ECode ZipFile::RAFStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return ReadBytesEx(buffer, 0, buffer->GetLength(), number);
}

ECode ZipFile::RAFStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode ZipFile::RAFStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

ZipFile::ZipInflaterInputStream::ZipInflaterInputStream(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inf,
    /* [in] */ Int32 bsize,
    /* [in] */ CZipEntry* entry)
    : mEntry(entry)
    , mBytesRead(0)
{
    ASSERT_SUCCEEDED(InflaterInputStream::Init(is, inf, bsize));
}

PInterface ZipFile::ZipInflaterInputStream::Probe(
/* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IInflaterInputStream) {
        return (IInflaterInputStream*)this;
    }
    else if (riid == EIID_IFilterInputStream) {
        return (IFilterInputStream*)this;
    }
    else if (riid == EIID_IInputStream) {
        return (IInputStream *)this;
    }
    return NULL;
}

UInt32 ZipFile::ZipInflaterInputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ZipFile::ZipInflaterInputStream::Release()
{
    return ElRefBase::Release();
}

ECode ZipFile::ZipInflaterInputStream::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ZipFile::ZipInflaterInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InflaterInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

//@Override
ECode ZipFile::ZipInflaterInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    FAIL_RETURN(InflaterInputStream::ReadBytesEx(buffer, off, nbytes, number));
    if (*number != -1) {
        mBytesRead += *number;
    }
    return NOERROR;
}

//@Override
ECode ZipFile::ZipInflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    if (mClosed) {
        // Our superclass will throw an exception, but there's a jtreg test that
        // explicitly checks that the InputStream returned from ZipFile.getInputStream
        // returns 0 even when closed.
        *number = 0;
        return NOERROR;
    }
    InflaterInputStream::Available(number);
    if (*number != 0) {
        Int64 size;
        mEntry->GetSize(&size);
        *number = size - mBytesRead;
    }
    return NOERROR;
}

ECode ZipFile::ZipInflaterInputStream::Close()
{
    return InflaterInputStream::Close();
}

ECode ZipFile::ZipInflaterInputStream::Mark(
    /* [in] */ Int32 mark)
{
    return InflaterInputStream::Mark(mark);
}

ECode ZipFile::ZipInflaterInputStream::IsMarkSupported(
    /* [in] */ Boolean *isMark)
{
    return InflaterInputStream::IsMarkSupported(isMark);
}

ECode ZipFile::ZipInflaterInputStream::Reset()
{
    return InflaterInputStream::Reset();
}

ECode ZipFile::ZipInflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return InflaterInputStream::Read(value);
}

ECode ZipFile::ZipInflaterInputStream::ReadBytes(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return ReadBytesEx(buffer, 0, buffer->GetLength(), number);
}

ECode ZipFile::ZipInflaterInputStream::Skip(
    /* [in] */ Int64 offset,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Skip(offset, number);
}

ZipFile::ZipFile()
{
}

ZipFile::~ZipFile()
{
    // if (guard != null) {
    //     guard.warnIfOpen();
    // }
    mEntries.Clear();
}

ECode ZipFile::Close()
{
    //guard.close();
    AutoPtr<IRandomAccessFile> raf = mRaf;

    if (raf != NULL) { // Only close initialized instances
        {
            Mutex::Autolock lock(mRafLock);
            mRaf = NULL;
            raf->Close();
        }
        if (mFileToDeleteOnClose != NULL) {
            Boolean result;
            mFileToDeleteOnClose->Delete(&result);
            mFileToDeleteOnClose = NULL;
        }
    }
    return NOERROR;
}

ECode ZipFile::CheckNotClosed()
{
   if (mRaf == NULL) {
       // throw new IllegalStateException("Zip file closed");
       return E_ILLEGAL_STATE_EXCEPTION;
   }
   return NOERROR;
}

ECode ZipFile::GetEntries(
    /* [out] */ IObjectContainer** entries)
{
    VALIDATE_NOT_NULL(entries);
    FAIL_RETURN(CheckNotClosed());

    FAIL_RETURN(CObjectContainer::New(entries));

    HashMap<String, AutoPtr<IZipEntry> >::Iterator it = mEntries.Begin();
    for (; it != mEntries.End(); ++it) {
        AutoPtr<IZipEntry> p = it->mSecond;
        (*entries)->Add(p.Get());
    }
    return NOERROR;
}

ECode ZipFile::GetEntry(
    /* [in] */ const String& entryName,
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry)
    FAIL_RETURN(CheckNotClosed());

    if (entryName.IsNull()) {
         return E_NULL_POINTER_EXCEPTION;
//         throw new NullPointerException("entryName == null");
    }

    HashMap<String, AutoPtr<IZipEntry> >::Iterator it = mEntries.Find(entryName);
    if (it == mEntries.End()) {
        it = mEntries.Find(entryName + "/");
    }
    if (it != mEntries.End()) {
        *entry = it->mSecond;
        INTERFACE_ADDREF(*entry)
    }
    return NOERROR;
}

ECode ZipFile::GetInputStream(
    /* [in] */ IZipEntry* entry,
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);
    *is = NULL;
    VALIDATE_NOT_NULL(entry);

    // Make sure this ZipEntry is in this Zip file.  We run it through the name lookup.
    String name;
    entry->GetName(&name);
    AutoPtr<IZipEntry> ze;
    GetEntry(name, (IZipEntry**)&ze);
    if (ze == NULL) {
        *is = NULL;
        return NOERROR;
    }

    // Create an InputStream at the right part of the file.
    AutoPtr<IRandomAccessFile> raf = mRaf;
    Mutex::Autolock lock(mRafLock);
    // We don't know the entry data's start position. All we have is the
    // position of the entry's local header. At position 28 we find the
    // length of the extra data. In some cases this length differs from
    // the one coming in the central header.
    AutoPtr<RAFStream> rafstrm = new RAFStream(raf,
            ((CZipEntry*)ze.Get())->mLocalHeaderRelOffset + 28);
    AutoPtr<IDataInputStream> dis;
    CDataInputStream::New((IInputStream*)rafstrm, (IDataInputStream**)&dis);
    AutoPtr<IDataInput> di = (IDataInput*)dis->Probe(EIID_IDataInput);
    //Int32 localExtraLenOrWhatever = Short.reverseBytes(is.readShort());
    Int16 value;
    di->ReadInt16(&value);
    Int32 localExtraLenOrWhatever = (Int16)((value << 8) | ((((UInt16)value) >> 8) & 0xFF));
    dis->Close();

    // Skip the name and this "extra" data or whatever it is:
    Int64 number;
    rafstrm->Skip(((CZipEntry*)ze.Get())->mNameLength + localExtraLenOrWhatever, &number);
    rafstrm->mLength = rafstrm->mOffset + ((CZipEntry*)ze.Get())->mCompressedSize;
    if (((CZipEntry*)ze.Get())->mCompressionMethod == IZipEntry::DEFLATED) {
        Int64 size;
        ze->GetSize(&size);
        Int32 bufSize = Elastos::Core::Math::Max(1024, (Int32)Elastos::Core::Math::Min(size, 65535ll));
        AutoPtr<IInflater> i;
        CInflater::New(TRUE, (IInflater**)&i);
        *is = (IInputStream*)new ZipInflaterInputStream(rafstrm, i, bufSize, (CZipEntry*)ze.Get());
    }
    else {
        *is = (IInputStream*)rafstrm;
    }

    INTERFACE_ADDREF(*is);
    return NOERROR;
}

ECode ZipFile::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mFileName;
    return NOERROR;
}

ECode ZipFile::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = -1;
    FAIL_RETURN(CheckNotClosed());
    *size = mEntries.GetSize();
    return NOERROR;
}

ECode ZipFile::ReadCentralDir()
{
    /*
     * Scan back, looking for the End Of Central Directory field.  If
     * the archive doesn't have a comment, we'll hit it on the first
     * try.
     *
     * No need to synchronize mRaf here -- we only do this when we
     * first open the Zip file.
     */
    Int64 len;
    mRaf->GetLength(&len);
    Int64 scanOffset = len - IZipConstants::ENDHDR;
    if (scanOffset < 0) {
        return E_ZIP_EXCEPTION;
//        throw new ZipException("File too short to be a zip file: " + mRaf.length());
    }
    Int64 stopOffset = scanOffset - 65536;
    if (stopOffset < 0) {
        stopOffset = 0;
    }

    AutoPtr<IDataInput> di = IDataInput::Probe(mRaf);
    const Int32 ENDHEADERMAGIC = 0x06054b50;
    Int32 value;
    while(TRUE) {
        mRaf->Seek(scanOffset);
        di->ReadInt32(&value);
        if (Elastos::Core::Math::ReverseBytes(value) == ENDHEADERMAGIC) {
            break;
        }

        scanOffset--;
        if (scanOffset < stopOffset) {
            return E_ZIP_EXCEPTION;
//            throw new ZipException("EOCD not found; not a Zip archive?");
        }
    }

    // Read the End Of Central Directory. We could use ENDHDR instead of the magic number 18,
    // but we don't actually need all the header.
    AutoPtr<ArrayOf<Byte> > eocd = ArrayOf<Byte>::Alloc(18);
    di->ReadFullyEx(eocd, 0, eocd->GetLength());

    // Pull out the information we need.
    AutoPtr<IHeapBufferIterator> it;
    CHeapBufferIterator::New(eocd, 0, eocd->GetLength(), ByteOrder_LITTLE_ENDIAN,
            (IHeapBufferIterator**)&it);

    Int16 temp16;
    it->ReadInt16(&temp16);
    Int32 diskNumber = temp16 & 0xffff;
    it->ReadInt16(&temp16);
    Int32 diskWithCentralDir = temp16 & 0xffff;
    it->ReadInt16(&temp16);
    Int32 numEntries = temp16 & 0xffff;
    it->ReadInt16(&temp16);
    Int32 totalNumEntries = temp16 & 0xffff;
    it->Skip(4); // Ignore centralDirSize.
    Int32 centralDirOffset;
    it->ReadInt32(&centralDirOffset);

    if (numEntries != totalNumEntries || diskNumber != 0 || diskWithCentralDir != 0) {
        return E_ZIP_EXCEPTION;
//        throw new ZipException("spanned archives not supported");
    }

    // Seek to the first CDE and read all entries.
    AutoPtr<RAFStream> rafs = new RAFStream(mRaf, centralDirOffset);
    AutoPtr<IBufferedInputStream> bin;
    FAIL_RETURN(CBufferedInputStream::New((IInputStream*)rafs, 4096,
            (IBufferedInputStream**)&bin));
    IInputStream* is = IInputStream::Probe(bin.Get());

    String name;
    AutoPtr<ArrayOf<Byte> > hdrBuf = ArrayOf<Byte>::Alloc(IZipFile::CENHDR); // Reuse the same buffer for each entry.
    for (Int32 i = 0; i < numEntries; ++i) {
        AutoPtr<IZipEntry> newEntry;
        FAIL_RETURN(CZipEntry::New(*hdrBuf, is, (IZipEntry**)&newEntry));
        newEntry->GetName(&name);
        mEntries[name] = newEntry;
    }

    return NOERROR;
}

ECode ZipFile::Init(
    /* [in] */ IFile* file)
{
    return Init(file, IZipFile::OPEN_READ);
}

ECode ZipFile::Init(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode)
{
    VALIDATE_NOT_NULL(file);
    file->GetPath(&mFileName);
    if (mode != IZipFile::OPEN_READ && mode != (IZipFile::OPEN_READ | IZipFile::OPEN_DELETE)) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((mode & IZipFile::OPEN_DELETE) != 0) {
        mFileToDeleteOnClose = file; // file.deleteOnExit();
    }
    else {
        mFileToDeleteOnClose = NULL;
    }

    FAIL_RETURN(CRandomAccessFile::New(file, String("r"), (IRandomAccessFile**)&mRaf));

    return ReadCentralDir();
//    guard.open("close");
}

ECode ZipFile::Init(
    /* [in] */ const String& name)
{
    AutoPtr<IFile> file;
    FAIL_RETURN(CFile::New(name, (IFile**)&file));
    return Init(file, IZipFile::OPEN_READ);
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
