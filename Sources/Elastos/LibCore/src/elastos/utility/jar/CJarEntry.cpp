
#include "CJarEntry.h"
#include "cmdef.h"
#include "CCodeSigner.h"
#include "CCertificateFactory.h"
#include "CCertificateFactoryHelper.h"
#include "CJarFile.h"
#include "JarVerifier.h"
#include "CLinkedList.h"
#include <cutils/log.h>

using Elastos::Security::IPrincipal;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::Cert::ICertPath;
using Elastos::Security::Cert::IX509Certificate;
using Elastos::Security::Cert::ICertificateFactoryHelper;
using Elastosx::Security::Auth::X500::IX500Principal;
using Elastos::Security::CCodeSigner;
using Elastos::Security::Cert::CCertificateFactory;
using Elastos::Security::Cert::CCertificateFactoryHelper;
using Elastos::Utility::Jar::JarVerifier;
using Elastos::Utility::Jar::CJarFile;
using Elastos::Utility::CLinkedList;
using Elastos::Utility::IList;
using Elastos::Utility::Zip::EIID_ZipEntry;

namespace Elastos {
namespace Utility {
namespace Jar {

CJarEntry::CJarEntry()
    : mIsFactoryChecked(FALSE)
{}

ECode CJarEntry::GetComment(
    /* [out] */ String* comment)
{
    VALIDATE_NOT_NULL(comment)
    *comment = ZipEntry::GetComment();
    return NOERROR;
}

ECode CJarEntry::GetCompressedSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size)
    *size = ZipEntry::GetCompressedSize();
    return NOERROR;
}

ECode CJarEntry::GetCrc(
    /* [out] */ Int64* checksum)
{
    VALIDATE_NOT_NULL(checksum)
    *checksum = ZipEntry::GetCrc();
    return NOERROR;
}

ECode CJarEntry::GetExtra(
    /* [out, callee] */ ArrayOf<Byte>** extra)
{
    VALIDATE_NOT_NULL(extra)
    *extra = ZipEntry::GetExtra();
    ARRAYOF_ADDREF(*extra)
    return NOERROR;
}

ECode CJarEntry::GetMethod(
    /* [out] */ Int32* method)
{
    VALIDATE_NOT_NULL(method)
    *method = ZipEntry::GetMethod();
    return NOERROR;
}

ECode CJarEntry::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = ZipEntry::GetName();
    return NOERROR;
}

ECode CJarEntry::GetSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size)
    *size = ZipEntry::GetSize();
    return NOERROR;
}

ECode CJarEntry::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time)
    *time = ZipEntry::GetTime();
    return NOERROR;
}

ECode CJarEntry::IsDirectory(
    /* [out] */ Boolean* isDirectory)
{
    VALIDATE_NOT_NULL(isDirectory)
    *isDirectory = ZipEntry::IsDirectory();
    return NOERROR;
}

ECode CJarEntry::SetComment(
    /* [in] */ const String& comment)
{
    return ZipEntry::SetComment(comment);
}

ECode CJarEntry::SetCompressedSize(
    /* [in] */ Int64 size)
{
    return ZipEntry::SetCompressedSize(size);
}

ECode CJarEntry::SetCrc(
    /* [in] */ Int64 checksum)
{
    return ZipEntry::SetCrc(checksum);
}

ECode CJarEntry::SetExtra(
    /* [in] */ ArrayOf<Byte>* data)
{
    return ZipEntry::SetExtra(data);
}

ECode CJarEntry::SetMethod(
    /* [in] */ Int32 method)
{
    return ZipEntry::SetMethod(method);
}

ECode CJarEntry::SetSize(
    /* [in] */ Int64 size)
{
    return ZipEntry::SetSize(size);
}

ECode CJarEntry::SetTime(
    /* [in] */ Int64 timeByMil)
{
    return ZipEntry::SetTime(timeByMil);
}

ECode CJarEntry::GetAttributes(
    /* [out] */ IAttributes** attrib)
{
    VALIDATE_NOT_NULL(attrib)
    if (mAttributes != NULL || !mParentJar) {
        *attrib = mAttributes;
        INTERFACE_ADDREF(*attrib)
        return NOERROR;
    }
    AutoPtr<IManifest> manifest;
    FAIL_RETURN(mParentJar->GetManifest((IManifest**)&manifest));
    if (!manifest) {
        *attrib = NULL;
        return NOERROR;
    }
    String name;
    GetName(&name);
    mAttributes = NULL;
    FAIL_RETURN(manifest->GetAttributes(name, (IAttributes**)&mAttributes));
    *attrib = mAttributes;
    INTERFACE_ADDREF(*attrib)
    return NOERROR;
}

/**
 * Returns an array of {@code Certificate} Objects associated with this
 * entry or {@code null} if none exists. Make sure that the everything is
 * read from the input stream before calling this method, or else the method
 * returns {@code null}.
 *
 * @return the certificate for this entry.
 * @see java.security.cert.Certificate
 */
ECode CJarEntry::GetCertificates(
        /* [out, callee] */ ArrayOf<ICertificate*>** certificates)
{
    VALIDATE_NOT_NULL(certificates)
    if (mParentJar == NULL) {
        *certificates = NULL;
        return NOERROR;
    }
    AutoPtr<JarVerifier> jarVerifier = ((CJarFile*)mParentJar.Get())->mVerifier;
    if (jarVerifier == NULL) {
        *certificates = NULL;
        return NOERROR;
    }
    String name;
    GetName(&name);
    return jarVerifier->GetCertificates(name, certificates);
}

ECode CJarEntry::SetAttributes(
        /* [in] */ IAttributes* attrib)
{
    mAttributes = attrib;
    return NOERROR;
}

ECode CJarEntry::constructor(
    /* [in] */ const String& name)
{
    return ZipEntry::Init(name);
}

ECode CJarEntry::constructor(
    /* [in] */ IZipEntry* zipEntry)
{
    return ZipEntry::Init(reinterpret_cast<ZipEntry*>(zipEntry->Probe(EIID_ZipEntry)));
}

ECode CJarEntry::constructor(
    /* [in] */ IJarEntry* jarEntry)
{
    FAIL_RETURN(ZipEntry::Init(reinterpret_cast<ZipEntry*>(jarEntry->Probe(EIID_ZipEntry))))
    mParentJar = ((CJarEntry*)jarEntry)->mParentJar;
    mAttributes = ((CJarEntry*)jarEntry)->mAttributes;
    mSigners = ((CJarEntry*)jarEntry)->mSigners;
    return NOERROR;
}

/**
 * Returns the code signers for the digital signatures associated with the
 * JAR file. If there is no such code signer, it returns {@code null}. Make
 * sure that the everything is read from the input stream before calling
 * this method, or else the method returns {@code null}.
 *
 * @return the code signers for the JAR entry.
 * @see CodeSigner
 */
ECode CJarEntry::GetCodeSigners(
        /* [out, callee] */ ArrayOf<ICodeSigner*>** codeSigner)
{
    VALIDATE_NOT_NULL(codeSigner)
    if (mSigners == NULL) {
       AutoPtr<ArrayOf<ICertificate*> > certificates;
       GetCertificates((ArrayOf<ICertificate*>**)&certificates);
       GetCodeSigners(certificates, (ArrayOf<ICodeSigner*>**)&mSigners);
    }
    if (mSigners == NULL) {
        *codeSigner = NULL;
        return NOERROR;
    }
    AutoPtr<ArrayOf<ICodeSigner*> > tmp = ArrayOf<ICodeSigner*>::Alloc(mSigners->GetLength());
    tmp->Copy(mSigners);
    *codeSigner = tmp;
    INTERFACE_ADDREF(*codeSigner)
    return NOERROR;
}

PInterface CJarEntry::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ZipEntry) {
        return reinterpret_cast<PInterface>((ZipEntry*)this);
    }

    return _CJarEntry::Probe(riid);
}

ECode CJarEntry::GetCodeSigners(
    /* [in] */ ArrayOf<ICertificate*>* certs,
    /* [out, callee] */ ArrayOf<ICodeSigner*>** codeSigners)
{
    if (certs == NULL) {
        *codeSigners = NULL;
        return NOERROR;
    }

    AutoPtr<IX500Principal> prevIssuer;
    Int32 length = certs->GetLength();
    AutoPtr< List<AutoPtr<ICertificate> > > list = new List<AutoPtr<ICertificate> >(length);
    AutoPtr<List<AutoPtr<ICodeSigner> > > asigners = new List<AutoPtr<ICodeSigner> >();

    for (Int32 i = 0; i < length; ++i) {
        if(!IX509Certificate::Probe((*certs)[i])) {
            // Only X509Certificate-s are taken into account - see API spec.
            continue;
        }
        AutoPtr<IX509Certificate> x509 = IX509Certificate::Probe((*certs)[i]);
        if (prevIssuer != NULL) {
            AutoPtr<IX500Principal> subj;
            x509->GetSubjectX500Principal((IX500Principal**)&subj);
            Boolean equal;
            if (!(IPrincipal::Probe(prevIssuer)->Equals(subj.Get(), &equal), equal)) {
                // Ok, this ends the previous chain,
                // so transform this one into CertPath ...
                AddCodeSigner(asigners, list);
                // ... and start a new one
                list->Clear();
            } // else { it's still the same chain }
        }
        prevIssuer = NULL;
        x509->GetIssuerX500Principal((IX500Principal**)&prevIssuer);
        list->PushBack(ICertificate::Probe(x509.Get()));
    }

    if (!list->IsEmpty()) {
        AddCodeSigner(asigners, list);
    }

    if (asigners->IsEmpty()) {
        // 'signers' is 'null' already
        *codeSigners = NULL;
        return NOERROR;
    }

    AutoPtr<ArrayOf<ICodeSigner*> > tmp = ArrayOf<ICodeSigner*>::Alloc(asigners->GetSize());
    Int32 iIndex = 0;
    for (List<AutoPtr<ICodeSigner> >::Iterator it = asigners->Begin(); it != asigners->End(); ++it) {
        tmp->Set(iIndex++, *it);
    }
    *codeSigners = tmp;
    INTERFACE_ADDREF(tmp)
    return NOERROR;
}

ECode CJarEntry::AddCodeSigner(
    /* [in] */ List<AutoPtr<ICodeSigner> >* asigners,
    /* [in] */ List<AutoPtr<ICertificate> >* list)
{
    AutoPtr<ICertPath> certPath;
    if (!mIsFactoryChecked) {
        AutoPtr<ICertificateFactoryHelper> helper;
        CCertificateFactoryHelper::AcquireSingleton((ICertificateFactoryHelper**)&helper);
        mFactory = NULL;
        ECode ec = helper->GetInstance(String("X.509"), (ICertificateFactory**)&mFactory);
        if (ec == E_CERTIFICATE_EXCEPTION) {
        }
        else if (FAILED(ec)) {
            return ec;
        }
        mIsFactoryChecked = TRUE;
    }
    if (mFactory == NULL) {
        return NOERROR;
    }
    AutoPtr<IList> lst;
    FAIL_RETURN(CLinkedList::New((IList**)&lst))
    List<AutoPtr<ICertificate> >::Iterator it = list->Begin();
    for (Int32 i = 0; it != list->End(); ++it, ++i)
    lst->AddEx(i, (*it).Get());
    ECode ec = mFactory->GenerateCertPathEx2(lst, (ICertPath**)&certPath);
    if (ec == E_CERTIFICATE_EXCEPTION) {
    } else if( FAILED(ec)) {
        return ec;
    }
    if (certPath != NULL) {
        AutoPtr<ICodeSigner> codeSigner;
        CCodeSigner::New(certPath, NULL, (ICodeSigner**)&codeSigner);
        asigners->PushBack(codeSigner);
    }
    return NOERROR;
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos

