
#include "CertStore.h"
#include "cmdef.h"

#ifdef ELASTOS_CORELIBRARY
#include "CSecurity.h"
#endif

using Elastos::Security::CSecurity;
using Elastos::Security::ISecurity;

namespace Elastos {
namespace Security {
namespace Cert {

const String CertStore::SERVICE = String("CertStore");

//Apache...Todo
// Used to access common engine functionality
//static const Engine ENGINE = new Engine(SERVICE);

const String CertStore::PROPERTYNAME = String("certstore.type");

// Default value of CertStore type. It returns if certpathbuild.type
// property is not defined in java.security file
const String CertStore::DEFAULTPROPERTY = String("LDAP");

ECode CertStore:: GetInstance(
    /* [in] */ const String& type,
    /* [in] */ ICertStoreParameters *params,
    /* [out] */ ICertStore **certStore)
{
    VALIDATE_NOT_NULL(certStore)
    if (type.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    //Apache...Todo
    /*
    try {
        Engine.SpiAndProvider sap = ENGINE.getInstance(type, params);
        return new CertStore((CertStoreSpi) sap.spi, sap.provider, type, params);
    } catch (NoSuchAlgorithmException e) {
        Throwable th = e.getCause();
        if (th == null) {
            throw e;
        } else {
            throw new InvalidAlgorithmParameterException(e.getMessage(), th);
        }
    }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CertStore:: GetInstance(
    /* [in] */ const String& type,
    /* [in] */ ICertStoreParameters *params,
    /* [in] */ const String& provider,
    /* [out] */ ICertStore **certStore)
{
    if (provider.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IProvider> impProvider;
    AutoPtr<ISecurity> security;
    CSecurity::AcquireSingleton((ISecurity**)&security);
    security->GetProvider(provider, (IProvider**)&impProvider);
    if (impProvider == NULL) {
        return E_NO_SUCH_PROVIDER_EXCEPTION;
    }
    return GetInstance(type, params, impProvider, certStore);
}

ECode CertStore:: GetInstance(
    /* [in] */ const String& type,
    /* [in] */ ICertStoreParameters *params,
    /* [in] */ IProvider *provider,
    /* [out] */ ICertStore **certStore)
{
    if (provider == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (type.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    //Apache...Todo later
    /*
    try {
        Object spi = ENGINE.getInstance(type, provider, params);
        return new CertStore((CertStoreSpi) spi, provider, type, params);
    } catch (NoSuchAlgorithmException e) {
        Throwable th = e.getCause();
        if (th == null) {
            throw e;
        } else {
            throw new InvalidAlgorithmParameterException(e.getMessage(), th);
        }
    }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CertStore:: GetDefaultType(
    /* [out] */ String *type)
{
    VALIDATE_NOT_NULL(type)
    String defaultType;
    AutoPtr<ISecurity> security;
    CSecurity::AcquireSingleton((ISecurity**)&security);
    security->GetProperty(PROPERTYNAME, &defaultType);
    *type = defaultType.IsNull() ? DEFAULTPROPERTY : defaultType;
    return NOERROR;
}

ECode CertStore::GetType(
    /* [out] */ String *type)
{
    VALIDATE_NOT_NULL(type)
    *type = mType;
    return NOERROR;
}

ECode CertStore::GetProvider(
    /* [out] */ IProvider **provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = mProvider;
    return NOERROR;
}

ECode CertStore::GetCertStoreParameters(
    /* [out] */ ICertStoreParameters **parameters)
{
    if (mCertStoreParams == NULL) {
        return NOERROR;
    } else {
        AutoPtr<IInterface> clonedObj;
        mCertStoreParams->Clone((IInterface**)&clonedObj);
        *parameters = clonedObj;
        INTERFACE_ADDREF(*parameters)
        return NOERROR;
    }
}

ECode CertStore::GetCertificates(
    /* [in] */ ICertSelector *selector,
    /* [out] */ ICollection **certs)
{
    return mSpiImpl->EngineGetCertificates(selector, (ICollection**)&certs);
}

ECode CertStore::GetCRLs(
    /* [in] */ ICRLSelector *selector,
    /* [out] */ ICollection **crls)
{
    return mSpiImpl->EngineGetCRLs(selector, (ICollection**)&crls);
}

CertStore::CertStore(
    /* [in] */ ICertStoreSpi* storeSpi,
    /* [in] */ IProvider* provider,
    /* [in] */ const String& type,
    /* [in] */ ICertStoreParameters* params)
    : mProvider(provider)
    , mType(type)
    , mSpiImpl(storeSpi)
    , mCertStoreParams(params)
{}

}
}
}

