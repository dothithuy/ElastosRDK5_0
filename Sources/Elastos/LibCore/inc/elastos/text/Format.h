#ifndef __FORMAT_H_
#define __FORMAT_H_

#include "Elastos.CoreLibrary_server.h"
#include "AttributedCharacterIteratorAttribute.h"
#include <elastos/StringBuffer.h>

using Elastos::Text::IAttributedCharacterIterator;;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;
using Elastos::Core::StringBuffer;
using Elastos::Core::IStringBuffer;

namespace Elastos {
namespace Text {

class Format
{
public:
    class Field : public AttributedCharacterIteratorAttribute
    {
    protected:
        CARAPI Init(
            /* [in] */ const String& fieldName)
        {
            return AttributedCharacterIteratorAttribute::Init(fieldName);
        }

        CARAPI GetClassID(
            /* [out] */ ClassID* clsid)
        {
            assert(0 && "TODO");
            return NOERROR;
            // return _Field::GetClassID(clsid);
        }
    };

public:
    //@Override
    //public Object clone();

    virtual ~Format() {}

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI FormatObject(
        /* [in] */ IInterface* object,
        /* [out] */ String* value);

    virtual CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value) = 0;

    virtual CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** characterIterator);

    virtual CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [out] */ IInterface** object);

    virtual CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object) = 0;

    virtual CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result) = 0;

protected:
    Format();

    static CARAPI UpTo(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [in] */ StringBuffer& buffer,
        /* [in] */ Char32 stop,
        /* [out] */ Boolean* succeeded);

    static CARAPI UpToWithQuotes(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [in] */ StringBuffer& buffer,
        /* [in] */ Char32 stop,
        /* [in] */ Char32 start,
        /* [out] */ Boolean* succeeded);
};

} // namespace Text
} // namespace Elastos

#endif //__FORMAT_H_
