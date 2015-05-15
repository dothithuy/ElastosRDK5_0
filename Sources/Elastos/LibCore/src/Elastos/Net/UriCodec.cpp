#include "UriCodec.h"
#include <Elastos.Core_server.h>
#include <elastos/Character.h>
#include <elastos/IntegralToString.h>
#include "CCharBufferHelper.h"
#include "CByteArrayOutputStream.h"
#include "CCharsets.h"

using Elastos::Core::Character;
using Elastos::Core::StringBuilder;
using Elastos::Core::IntegralToString;
using Elastos::IO::ICharBuffer;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::CCharBufferHelper;
using Elastos::IO::ICharBufferHelper;
using Elastos::IO::CByteArrayOutputStream;
using Elastos::IO::Charset::ICharsets;
using Elastos::IO::Charset::CCharsets;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Net {

ECode UriCodec::Validate(
    /* [in] */ const String& uri,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& name,
    /* [out] */ String* result) const
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<ArrayOf<Char32> > char32Array = uri.GetChars();
    for(Int32 i = start; i < end;) {
        Char32 ch = (*char32Array)[i];
        if ((ch >= 'a' && ch <= 'z')
            || (ch >= 'A' && ch <= 'Z')
            || (ch >= '0' && ch <= '9')
            || IsRetained(ch)){
            ++i;
        }
        else if (ch == '%'){
            if(i + 2 >= end){
                return E_URI_SYNTAX_EXCEPTION;
            }

            Int32 d1 = HexToInt((*char32Array)[i+1]);
            Int32 d2 = HexToInt((*char32Array)[i+2]);
            if (d1 == -1 || d2 == -1) {
                return E_URI_SYNTAX_EXCEPTION;
            }
            i += 3;
        }
        else{
            return E_URI_SYNTAX_EXCEPTION;
        }
    }
    *result = uri.Substring(start, end);
    return NOERROR;
}

ECode UriCodec::ValidateSimple(
    /* [in] */ const String& s,
    /* [in] */ const String& legal)
{
    AutoPtr<ArrayOf<Char32> > char32Array = s.GetChars();
    for(Int32 i = 0; i < char32Array->GetLength(); i++) {
        Char32 ch = (*char32Array)[i];
        if(!((ch >= 'a' && ch <= 'z')
                || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9')
                || legal.IndexOf(ch) > -1)) {
            return E_URI_SYNTAX_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode UriCodec:: AppendEncoded(
    /* [in] */ StringBuilder& builder,
    /* [in] */ const String& s) const
{
    AutoPtr<ICharset> charset = GetDefaultCharset();
    return AppendEncodedEx(builder, s, charset, FALSE);
}

ECode UriCodec::AppendEncodedEx(
    /* [in] */ StringBuilder& builder,
    /* [in] */ const String& s,
    /* [in] */ ICharset* charset,
    /* [in] */ Boolean isPartiallyEncoded) const
{
    if (s.IsNull())
        return E_NULL_POINTER_EXCEPTION;

    Int32 escapeStart = -1;
    AutoPtr<ArrayOf<Char32> > char32Array = s.GetChars();
    for(Int32 i = 0; i < s.GetLength(); i++) {
        Char32 c = (*char32Array)[i];
        if((c >= 'a' && c <= 'z')
                || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9')
                || IsRetained(c)
                || (c == '%') && isPartiallyEncoded) {
            if (escapeStart != -1) {
                AppendHexEx(builder, s.Substring(escapeStart, i), charset);
                escapeStart = -1;
            }

            if (c =='%' && isPartiallyEncoded){
                builder.AppendString(s.Substring(i, i + 3));
                i += 2;
            }
            else if (c == ' '){
                builder.AppendChar('+');
            }
            else{
                builder.AppendChar(c);
            }
        }
        else if(escapeStart == -1) {
            escapeStart = i;
        }
    }

    if (escapeStart != -1) {
        AppendHexEx(builder, s.Substring(escapeStart, s.GetLength()), charset);
    }
    return NOERROR;
}

ECode UriCodec::Encode(
    /* [in] */ const String& s,
    /* [in] */ ICharset* charset,
    /* [out] */ String* result)
{
    // Guess a bit larger for encoded form
    StringBuilder builder(s.GetByteLength() + 16);
    AppendEncodedEx(builder, s, charset, FALSE);
    *result = builder.ToString();
    return NOERROR;
}

ECode UriCodec::AppendPartiallyEncoded(
    /* [in] */ StringBuilder& builder,
    /* [in] */ const String& s)
{
    AutoPtr<ICharset> charset = GetDefaultCharset();
    AppendEncodedEx(builder, s, charset, TRUE);
    return NOERROR;
}

ECode UriCodec::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharset> charset = GetDefaultCharset();
    return DecodeEx(s, FALSE, charset, TRUE, result);
}

AutoPtr<ArrayOf<Byte> > UriCodec::GetBytes(
    /* [in] */ const char* cPtr,
    /* [in] */ ICharset* charSet)
{
    String buf(cPtr);
    AutoPtr<ArrayOf<Char32> > char32Array = buf.GetChars();
    AutoPtr<ICharsets> charSets;
    CCharsets::AcquireSingleton((ICharsets**)&charSets);
    AutoPtr<ArrayOf<Byte> > byteArray;
    String canonicalCharsetName;
    charSet->GetName(&canonicalCharsetName);
    if(canonicalCharsetName == String("UTF-8")) {
        charSets->ToUtf8Bytes(*char32Array, 0, (Int32)(char32Array->GetLength()),
            (ArrayOf<Byte>**)&byteArray);
    }
    else if(canonicalCharsetName == String("ISO-8859-1")) {
        charSets->ToIsoLatin1Bytes(*char32Array, 0, (Int32)(char32Array->GetLength()),
            (ArrayOf<Byte>**)&byteArray);
    }
    else if(canonicalCharsetName == String("US-ASCII")) {
        charSets->ToAsciiBytes(*char32Array, 0, (Int32)(char32Array->GetLength()),
            (ArrayOf<Byte>**)&byteArray);
    }
    else{
        AutoPtr<ICharBufferHelper> charBufferHelper;
        CCharBufferHelper::AcquireSingleton((ICharBufferHelper**)&charBufferHelper);
        AutoPtr<ICharBuffer> chars;
        AutoPtr<ICharBuffer> charsBuf;
        AutoPtr<IByteBuffer> byteBuffer;

        charBufferHelper->WrapArray(char32Array, (ICharBuffer**)&chars);
        chars->AsReadOnlyBuffer((ICharBuffer**)&charsBuf);
        charSet->Encode(charsBuf, (IByteBuffer**)&byteBuffer);
        Int32 len;
        byteBuffer->GetLimit(&len);
        byteArray= ArrayOf<Byte>::Alloc(len);
        byteBuffer->GetBytes(byteArray);
    }
    return byteArray;
}

ECode UriCodec::DecodeEx(
    /* [in] */ const String& s,
    /* [in] */ Boolean convertPlus,
    /* [in] */ ICharset* charset,
    /* [in] */ Boolean throwOnFailure,
    /* [out] */ String* decodedString)
{
    VALIDATE_NOT_NULL(decodedString);
    if (s.IndexOf('%') == -1 && (!convertPlus || s.IndexOf('+') == -1)) {
        *decodedString = s;
        return NOERROR;
    }

    StringBuilder result(s.GetByteLength());
    AutoPtr<IByteArrayOutputStream> out;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&out);
    AutoPtr<ArrayOf<Char32> > char32Array = s.GetChars();
    for (Int32 i = 0; i < s.GetLength();) {
        Char32 c = (*char32Array)[i];
        if (c == '%') {
            do {
                Int32 d1, d2;
                if (i + 2 < s.GetLength()
                        && (d1 = HexToInt((*char32Array)[i + 1])) != -1
                        && (d2 = HexToInt((*char32Array)[i + 2])) != -1) {
                    IOutputStream::Probe(out)->Write((Byte) ((d1 << 4) + d2));
                }
                else if (throwOnFailure) {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
                else {
                    char* chars = "\ufffd";
                    AutoPtr<ArrayOf<Byte> > replacement = GetBytes(chars, charset);
                    out->WriteBytesEx(*replacement, 0, replacement->GetLength());
                }
                i += 3;
            } while (i < s.GetLength() && (*char32Array)[i] == '%');

            AutoPtr<ArrayOf<Byte> > bytes;
            out->ToByteArray((ArrayOf<Byte>**)&bytes);
            //result.append(new String(out.toByteArray(), charset);
            result.AppendString(String((char*)bytes->GetPayload()));
            out->Reset();
        }
        else {
            if (convertPlus && c == '+') {
                c = ' ';
            }
            result.AppendChar(c);
            i++;
        }
    }
    *decodedString = result.ToString();
    return NOERROR;
}

Int32 UriCodec::HexToInt(
    /* [in] */ char c)
{
    if('0' <= c && c <= '9'){
        return  c - '0';
    }else if('a' <= c && c <= 'f'){
        return 10 + (c - 'a');
    }else if('A' <= c && c <= 'F'){
        return 10 + (c - 'A');
    }else{
        return -1;
    }
}

ECode UriCodec::AppendHex(
    /* [in] */ StringBuilder& builder,
    /* [in] */ Byte b)
{
    builder.AppendChar('%');
    builder.AppendString(IntegralToString::ByteToHexString(b, TRUE));
    return NOERROR;
}

ECode UriCodec::AppendHexEx(
    /* [in] */ StringBuilder& builder,
    /* [in] */ const String& s,
    /* [in] */ ICharset* charset)
{
    AutoPtr<ArrayOf<Byte> > bytes;
    bytes = GetBytes(s.string(), charset);
    assert(bytes != NULL);
    for (Int32 i = 0; i < bytes->GetLength(); ++i) {
        AppendHex(builder, (*bytes)[i]);
    }
    return NOERROR;
}

AutoPtr<ICharset> UriCodec::GetDefaultCharset()
{
    AutoPtr<ICharset> charset;
    AutoPtr<ICharsets> charsets;
    CCharsets::AcquireSingleton((ICharsets**)&charsets);
    charsets->GetUTF_8((ICharset**)&charset);
    return charset;
}

} // namespace Net
} // namespace Elastos