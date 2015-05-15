
#ifndef __ORG_SAX2_CDRIVER_H__
#define __ORG_SAX2_CDRIVER_H__

#include "cmdef.h"
#include "_CDriver.h"
#include "Org.Xml.Sax.h"
#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#include "CURL.h"
#include "CInputSource.h"
#include "CDefaultHandler.h"
#else
#include "Elastos.Core.h"
#endif

using Org::Xml::Sax::IEntityResolver;
using Org::Xml::Sax::IDTDHandler;
using Org::Xml::Sax::IContentHandler;
using Org::Xml::Sax::IErrorHandler;
using Org::Xml::Sax::IInputSource;

namespace Org {
namespace Xmlpull {
namespace V1 {
namespace Sax2 {

CarClass(CDriver)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IXmlPullParser* pp);

    CARAPI GetPublicId(
        /* [out] */ String* id);

    CARAPI GetSystemId(
        /* [out] */ String* id);

    CARAPI GetLineNumber(
        /* [out] */ Int32* lineNumber);

    CARAPI GetColumnNumber(
        /* [out] */ Int32* columnNumber);

    CARAPI GetFeature(
        /* [in] */ const String& name,
        /* [out] */ Boolean* feature);

    CARAPI SetFeature(
        /* [in] */ const String& name,
        /* [in] */ Boolean value);

    CARAPI GetProperty(
        /* [in] */ const String& name,
        /* [out] */ IInterface** value);

    CARAPI SetProperty(
        /* [in] */ const String& name,
        /* [in] */ IInterface* value);

    CARAPI SetEntityResolver(
        /* [in] */ IEntityResolver* resolver);

    CARAPI GetEntityResolver(
        /* [out] */ IEntityResolver** resolver);

    CARAPI SetDTDHandler(
        /* [in] */ IDTDHandler* handler);

    CARAPI GetDTDHandler(
        /* [out] */ IDTDHandler** handler);

    CARAPI SetContentHandler(
        /* [in] */ IContentHandler* handler);

    CARAPI GetContentHandler(
        /* [out] */ IContentHandler** handler);

    CARAPI SetErrorHandler(
        /* [in] */ IErrorHandler* handler);

    CARAPI GetErrorHandler(
        /* [out] */ IErrorHandler** handler);

    CARAPI Parse(
        /* [in] */ IInputSource* source);

    CARAPI ParseEx(
        /* [in] */ const String& systemId);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetURI(
        /* [in] */ Int32 index,
        /* [out] */ String* URI);

    CARAPI GetLocalName(
        /* [in] */ Int32 index,
        /* [out] */ String* localName);

    CARAPI GetQName(
        /* [in] */ Int32 index,
        /* [out] */ String* qName);

    CARAPI GetType(
        /* [in] */ Int32 index,
        /* [out] */ String* type);

    CARAPI GetValue(
        /* [in] */ Int32 index,
        /* [out] */ String* value);

    CARAPI GetIndex(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ Int32* index);

    CARAPI GetIndexEx(
        /* [in] */ const String& qName,
        /* [out] */ Int32* index);

    CARAPI GetTypeEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String* type);

    CARAPI GetTypeEx2(
        /* [in] */ const String& qName,
        /* [out] */ String* type);

    CARAPI GetValueEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String* value);

    CARAPI GetValueEx2(
        /* [in] */ const String& qName,
        /* [out] */ String* value);

    CARAPI_(void) ParseSubTree(
        /* [in] */ IXmlPullParser* pp);

protected:
    CARAPI_(void) StartElement(
        /* [in] */ const String& namesp,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName);

protected:
    static const String DECLARATION_HANDLER_PROPERTY; // = "http://xml.org/sax/properties/declaration-handler";

    static const String LEXICAL_HANDLER_PROPERTY; // = "http://xml.org/sax/properties/lexical-handler";

    static const String NAMESPACES_FEATURE; // = "http://xml.org/sax/features/namespaces";

    static const String NAMESPACE_PREFIXES_FEATURE; // = "http://xml.org/sax/features/namespace-prefixes";

    static const String VALIDATION_FEATURE; // = "http://xml.org/sax/features/validation";

    static const String APACHE_SCHEMA_VALIDATION_FEATURE; // = "http://apache.org/xml/features/validation/schema";

    static const String APACHE_DYNAMIC_VALIDATION_FEATURE; // = "http://apache.org/xml/features/validation/dynamic";

    AutoPtr<IContentHandler> mContentHandler; // = new DefaultHandler();
    AutoPtr<IErrorHandler> mErrorHandler; // = new DefaultHandler();

    String mSystemId;

    AutoPtr<IXmlPullParser> mPp;
};

} // namespace Sax2
} // namespace V1
} // namespace Xmlpull
} // namespace Org

#endif // __ORG_SAX2_CDRIVER_H__