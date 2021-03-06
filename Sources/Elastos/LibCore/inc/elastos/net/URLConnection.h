
#ifndef __URLCONNECTION_H__
#define __URLCONNECTION_H__

#include "cmdef.h"
#include "Elastos.CoreLibrary_server.h"
#include <elautoptr.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::Utility::HashMap;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Net {

// extern "C" const InterfaceID EIID_URLConnection;

class URLConnection
{
public:
    class DefaultContentHandler
        : public ElRefBase
        , public IContentHandler
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetContent(
            /* [in] */ IURLConnection* uConn,
            /* [out] */ IInterface** obj);

        CARAPI GetContentEx(
            /* [in] */ IURLConnection* uConn,
            /* [in] */ const ArrayOf<InterfaceID>& types,
            /* [out] */ IInterface** obj);
    };

public:
    virtual CARAPI Connect() = 0;

    virtual CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean* value);

    virtual CARAPI GetContent(
        /* [out] */ IInterface** content);

    // Param is not generic in spec
//        public Object getContent(Class[] types);

    virtual CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    virtual CARAPI GetContentLength(
        /* [out] */ Int32* length);

    virtual CARAPI GetContentType(
        /* [out] */ String* type);

    virtual CARAPI GetDate(
        /* [out] */ Int64* date);

    static CARAPI_(Boolean) GetDefaultAllowUserInteraction();

    static CARAPI_(String) GetDefaultRequestProperty(
        /* [in] */ const String& field);

    virtual CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean* value);

    virtual CARAPI GetDoInput(
        /* [out] */ Boolean* allowed);

    virtual CARAPI GetDoOutput(
        /* [out] */ Boolean* allowed);

    virtual CARAPI GetExpiration(
        /* [out] */ Int64* expiration);

    static CARAPI_(AutoPtr<IFileNameMap>) GetFileNameMap();

    virtual CARAPI GetHeaderFieldByPosition(
        /* [in] */ Int32 pos,
        /* [out] */ String* value);

//    public Map<String, List<String>> getHeaderFields()

//    public Map<String, List<String>> getRequestProperties()

    virtual CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    virtual CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    virtual CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    virtual CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& field,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    virtual CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String* key);

    virtual CARAPI GetIfModifiedSince(
        /* [out] */ Int64* time);

    virtual CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    virtual CARAPI GetLastModified(
        /* [out] */ Int64* value);

    virtual CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

//        public java.security.Permission getPermission();

    virtual CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String* property);

    virtual CARAPI GetURL(
        /* [out] */ IURL** url);

    virtual CARAPI GetUseCaches(
        /* [out] */ Boolean* allowed);

    static CARAPI_(String) GuessContentTypeFromName(
        /* [in] */ const String& url);

    static CARAPI_(String) GuessContentTypeFromStream(
        /* [in] */ IInputStream* is);

    virtual CARAPI SetAllowUserInteraction(
        /* [in] */ Boolean newValue);

    static CARAPI SetContentHandlerFactory(
        /* [in] */ IContentHandlerFactory* contentFactory);

    static CARAPI_(void) SetDefaultAllowUserInteraction(
        /* [in] */ Boolean allows);

    static CARAPI_(void) SetDefaultRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& value);

    virtual CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetDoOutput(
        /* [in] */ Boolean newValue);

    static CARAPI_(void) SetFileNameMap(
        /* [in] */ IFileNameMap* map);

    virtual CARAPI SetIfModifiedSince(
        /* [in] */ Int64 newValue);

    virtual CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    virtual CARAPI SetUseCaches(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetConnectTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI GetConnectTimeout(
        /* [out] */ Int32* timeout);

    virtual CARAPI SetReadTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI GetReadTimeout(
        /* [out] */ Int32* timeout);

protected:
    URLConnection();

    virtual ~URLConnection();

    CARAPI Init(
        /* [in] */ IURL* url);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

private:
    CARAPI GetContentHandler(
        /* [in] */ const String& type,
        /* [out] */ IContentHandler** contentHandler);

    CARAPI CheckNotConnected();

    CARAPI_(String) ParseTypeString(
        /* [in] */ const String& typeString);

public:
    AutoPtr<IContentHandler> mDefaultHandler;

    // static Hashtable<String, Object> contentHandlers = new Hashtable<String, Object>();
    static HashMap<String, AutoPtr<IContentHandler> > sContentHandlers;

protected:
    AutoPtr<IURL> mUrl;

    Int64 mIfModifiedSince;

    Boolean mUseCaches;// = defaultUseCaches;

    Boolean mConnected;

    Boolean mDoOutput;

    Boolean mDoInput;

    Boolean mAllowUserInteraction;

private:
    String mContentType;

    static Boolean sDefaultAllowUserInteraction;

    static Boolean sDefaultUseCaches;

    Int64 mLastModified;

    static AutoPtr<IContentHandlerFactory> sContentHandlerFactory;

    Int32 mReadTimeout;

    Int32 mConnectTimeout;

    static AutoPtr<IFileNameMap> sFileNameMap;

    static Mutex sLock;
};

} // namespace Net
} // namespace Elastos

#endif //__URLCONNECTION_H__
