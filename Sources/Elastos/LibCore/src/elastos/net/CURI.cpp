
#include "cmdef.h"
#include "CURI.h"
#include "InetAddress.h"
#include "CURL.h"
#include "UrlUtils.h"
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>
#include <elastos/Character.h>

namespace Elastos {
namespace Net {

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

const String CURI::UNRESERVED = String("_-!.~\'()*");
const String CURI::PUNCTUATION = String(",;:$&+=");
const UriCodec& CURI::USER_INFO_ENCODER = CURI::PartEncoder(String(""));
const UriCodec& CURI::PATH_ENCODER = CURI::PartEncoder(String("/@"));
const UriCodec& CURI::AUTHORITY_ENCODER = CURI::PartEncoder(String("@[]"));
const UriCodec& CURI::FILE_AND_QUERY_ENCODER = CURI::PartEncoder(String("/@?"));
const UriCodec& CURI::ALL_LEGAL_ENCODER = CURI::PartEncoder(String("?/[]@"));
const UriCodec& CURI::ASCII_ONLY = CURI::ASCIIEncoder();
//AutoPtr<INetworkSystem> InitNetworkSystem()
//{
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    AutoPtr<IPlatform> platform;
//    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    AutoPtr<INetworkSystem> networkSystem;
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    platform->GetNetworkSystem((INetworkSystem**)&networkSystem);
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    return networkSystem;
//}
//
//AutoPtr<INetworkSystem> CURI::NETWORK_SYSTEM = InitNetworkSystem();

CURI::CURI()
    : mPort(-1)
    , mOpaque(FALSE)
    , mAbsolute(FALSE)
    , mServerAuthority(FALSE)
    , mHash(-1)
{
}

ECode CURI::constructor()
{
    return NOERROR;
}

ECode CURI::constructor(
    /* [in] */ const String& uri)
{
    return ParseURI(uri, FALSE);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& schemeSpecificPart,
    /* [in] */ const String& frag)
{
    StringBuilder uri;
    if (!scheme.IsNull()) {
        uri += scheme;
        uri += ':';
    }
    if (!schemeSpecificPart.IsNull()) {
        // QUOTE ILLEGAL CHARACTERS
        FAIL_RETURN(ALL_LEGAL_ENCODER.AppendEncoded(uri, schemeSpecificPart));
    }
    if (!frag.IsNull()) {
        uri += '#';
        // QUOTE ILLEGAL CHARACTERS
        FAIL_RETURN(ALL_LEGAL_ENCODER.AppendEncoded(uri, frag));
    }

    String uriStr;
    uri.ToString(&uriStr);
    return ParseURI(uriStr, FALSE);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& userInfo,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& path,
    /* [in] */ const String& query,
    /* [in] */ const String& fragment)
{
    if (scheme.IsNull() && userInfo.IsNull() && host.IsNull() && path.IsNull()
            && query.IsNull() && fragment.IsNull()) {
        mPath = String("");
        return NOERROR;
    }

    if (!scheme.IsNull() && !path.IsNullOrEmpty()
            && path.GetChar(0) != '/') {
//        throw new URISyntaxException(path, "Relative path");
        return E_URI_SYNTAX_EXCEPTION;
    }

    StringBuilder uri;
    if (!scheme.IsNull()) {
        uri += scheme;
        uri += ':';
    }

    if (!userInfo.IsNull() || !host.IsNull() || port != -1) {
        uri += "//";
    }

    if (!userInfo.IsNull()) {
        // QUOTE ILLEGAL CHARACTERS in userInfo
        FAIL_RETURN(USER_INFO_ENCODER.AppendEncoded(uri, userInfo));
        uri += '@';
    }

    if (!host.IsNull()) {
        // check for IPv6 addresses that hasn't been enclosed
        // in square brackets
        String temp = host;
        if (host.IndexOf(':') != -1 && host.IndexOf(']') == -1
                && host.IndexOf('[') == -1) {
//            host = "[" + host + "]";
            temp = String("[") + host + String("]");
        }
        uri += temp;
    }

    if (port != -1) {
        uri += ':';
        uri += port;
    }

    if (!path.IsNull()) {
        // QUOTE ILLEGAL CHARS
        FAIL_RETURN(PATH_ENCODER.AppendEncoded(uri, path));
    }

    if (!query.IsNull()) {
        uri += '?';
        // QUOTE ILLEGAL CHARS
        FAIL_RETURN(ALL_LEGAL_ENCODER.AppendEncoded(uri, query));
    }

    if (!fragment.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += '#';
        FAIL_RETURN(ALL_LEGAL_ENCODER.AppendEncoded(uri, fragment));
    }

    String uriStr;
    uri.ToString(&uriStr);
    return ParseURI(uriStr, TRUE);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& host,
    /* [in] */ const String& path,
    /* [in] */ const String& fragment)
{
    return constructor(scheme, String(NULL), host, -1, path, String(NULL), fragment);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& authority,
    /* [in] */ const String& path,
    /* [in] */ const String& query,
    /* [in] */ const String& fragment)
{
    if (!scheme.IsNull() && !path.IsNull() && !path.IsEmpty()
            && path.GetChar(0) != '/') {
//        throw new URISyntaxException(path, "Relative path");
        return E_URI_SYNTAX_EXCEPTION;
    }

    StringBuilder uri;
    if (!scheme.IsNull()) {
        uri += scheme;
        uri += ":";
    }
    if (!authority.IsNull()) {
        uri += "//";
        // QUOTE ILLEGAL CHARS
        FAIL_RETURN(AUTHORITY_ENCODER.AppendEncoded(uri, authority));
    }

    if (!path.IsNull()) {
        // QUOTE ILLEGAL CHARS
        FAIL_RETURN(PATH_ENCODER.AppendEncoded(uri, path));
    }
    if (!query.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += "?";
        FAIL_RETURN(ALL_LEGAL_ENCODER.AppendEncoded(uri, query));
    }
    if (!fragment.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += "#";
        FAIL_RETURN(ALL_LEGAL_ENCODER.AppendEncoded(uri, fragment));
    }
    String uriStr;
    uri.ToString(&uriStr);
    return ParseURI(uriStr, FALSE);
}

ECode CURI::ParseURI(
    /* [in] */ const String& uri,
    /* [in] */ Boolean forceServer)
{
    // assign uri string to the input value per spec
    mString = uri;
    Int32 fragmentStart = UrlUtils::FindFirstOf(uri, String("#"), 0, uri.GetLength());

    //"fragment"
    if(fragmentStart < uri.GetLength()) {
        FAIL_RETURN(ALL_LEGAL_ENCODER.Validate(uri, fragmentStart + 1, uri.GetLength(),
            String("fragment"), &mFragment));
    }

    // "scheme"
    Int32 start = 0;
    Int32 colon = UrlUtils::FindFirstOf(uri, String(":"), 0, fragmentStart);
    if(colon < UrlUtils::FindFirstOf(uri, String("/?#"), 0, fragmentStart)) {
        mAbsolute = TRUE;
        FAIL_RETURN(ValidateScheme(uri, colon, &mScheme));
        start = colon + 1;

        if (start == fragmentStart) {
            return E_URI_SYNTAX_EXCEPTION;
        }

        // URIs with schemes followed by a non-/ char are opaque and need no further parsing.
        if (!uri.RegionMatches(start, String("/"), 0, 1)) {
            mOpaque = TRUE;
            FAIL_RETURN(ALL_LEGAL_ENCODER.Validate(uri, start, fragmentStart,
                    String("scheme specific part"), &mSchemeSpecificPart));
            return NOERROR;
        }
    } else {
        mAbsolute = FALSE;
        start = 0;
    }

    mOpaque = FALSE;
    mSchemeSpecificPart = uri.Substring(start, fragmentStart);

    // "//authority"
    Int32 fileStart;
    if (uri.RegionMatches(start, String("//"), 0, 2)) {
        Int32 authorityStart = start + 2;
        fileStart = UrlUtils::FindFirstOf(uri, String("/?"), authorityStart, fragmentStart);
        if (authorityStart == uri.GetLength()) {
            return E_URI_SYNTAX_EXCEPTION;
        }
        if (authorityStart < fileStart) {
            FAIL_RETURN(AUTHORITY_ENCODER.Validate(uri, authorityStart,
                    fileStart, String("authority"), &mAuthority));
        }
    } else {
        fileStart = start;
    }

    // "path"
    Int32 queryStart = UrlUtils::FindFirstOf(uri, String("?"), fileStart, fragmentStart);
    FAIL_RETURN(PATH_ENCODER.Validate(uri, fileStart, queryStart, String("path"), &mPath));

    // "?query"
    if (queryStart < fragmentStart) {
        FAIL_RETURN(ALL_LEGAL_ENCODER.Validate(uri, queryStart + 1, fragmentStart, String("query"), &mQuery));
    }

    return ParseAuthority(forceServer);
}

ECode CURI::ValidateScheme(
    /* [in] */ const String& uri,
    /* [in] */ Int32 end,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    if (end == 0) {
        return E_URI_SYNTAX_EXCEPTION;
    }

    AutoPtr<ArrayOf<Char32> > char32Array = uri.GetChars();
    for (Int32 i = 0; i < end; i++) {
        if (!UrlUtils::IsValidSchemeChar(i, (*char32Array)[i])) {
            return E_URI_SYNTAX_EXCEPTION;
        }
    }
    *result = uri.Substring(0, end);
    return NOERROR;
}

ECode CURI::ParseAuthority(
    /* [in] */ Boolean forceServer)
{
    if (mAuthority.IsNull()) {
        return NOERROR;
    }

    String tempUserInfo;
    String temp = mAuthority;
    Int32 index = temp.IndexOf('@');
    Int32 hostIndex = 0;
    if (index != -1) {
        // remove user info
        tempUserInfo = temp.Substring(0, index);
        FAIL_RETURN(ValidateUserInfo(mAuthority, tempUserInfo, 0));
        temp = temp.Substring(index + 1); // host[:port] is left
        hostIndex = index + 1;
    }

    index = temp.LastIndexOf(':');
    Int32 endIndex = temp.IndexOf(']');

    String tempHost;
    Int32 tempPort = -1;
    if (index != -1 && endIndex != -1 && endIndex < index) {
        // determine port and host
        tempHost = temp.Substring(0, index);

        if ((UInt32)index < (temp.GetLength() - 1)) { // port part is not empty
//            try {
            tempPort = StringUtils::ParseInt32(temp.Substring(index + 1));
            if (tempPort < 0) {
                if (forceServer) {
                    return E_URI_SYNTAX_EXCEPTION;
                }
                return NOERROR;
            }
        // } catch (NumberFormatException e) {
        //     if (forceServer) {
        //         return E_URI_SYNTAX_EXCEPTION;
        //     }
        //     return NOERROR;
        // }
    }
    } else {
        tempHost = temp;
    }

    if (tempHost.IsEmpty()) {
        if (forceServer) {
            return E_URI_SYNTAX_EXCEPTION;
        }
        return NOERROR;
    }

    Boolean isValid = FALSE;
    FAIL_RETURN(IsValidHost(forceServer, tempHost, &isValid));
    if (!isValid) {
        return NOERROR;
    }

    // this is a server based uri,
    // fill in the userInfo, host and port fields
    mUserInfo = tempUserInfo;
    mHost = tempHost;
    mPort = tempPort;
    mServerAuthority = TRUE;

    return NOERROR;
}

ECode CURI::ValidateUserInfo(
    /* [in] */ const String& uri,
    /* [in] */ const String& userInfo,
    /* [in] */ Int32 index)
{
    AutoPtr<ArrayOf<Char32> > char32Array = userInfo.GetChars();
    if (char32Array) {
        for (Int32 i = 0; i < char32Array->GetLength(); i++) {
            if ((*char32Array)[i] == ']' || (*char32Array)[i] == '[') {
    //            throw new URISyntaxException(uri, "Illegal character in userInfo", index + i);
                return E_URI_SYNTAX_EXCEPTION;
            }
        }
    }
    return NOERROR;
}

ECode CURI::IsValidHost(
    /* [in] */ Boolean forceServer,
    /* [in] */ const String& host,
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid)

    if (host.StartWith("[")) {
        // IPv6 address
        if (!host.EndWith("]")) {
//            throw new URISyntaxException(host,
//                    "Expected a closing square bracket for IPv6 address", 0);
            return E_URI_SYNTAX_EXCEPTION;
        }
        Boolean isNumeric = FALSE;
        if(InetAddress::IsNumeric(host, &isNumeric), isNumeric) {
            *isValid = TRUE;
            return NOERROR;
        }

        return E_URI_SYNTAX_EXCEPTION;
    }

    // '[' and ']' can only be the first char and last char
    // of the host name
    if (host.IndexOf('[') != -1 || host.IndexOf(']') != -1) {
//        throw new URISyntaxException(host, "Illegal character in host name", 0);
        return E_URI_SYNTAX_EXCEPTION;
    }

    Int32 index = host.LastIndexOf('.');
    if (index < 0 || (UInt32)index == host.GetLength() - 1
            || !Character::IsDigit(host.GetChar(index + 1))) {
        // domain name
        if (IsValidDomainName(host)) {
            *isValid = TRUE;
            return NOERROR;
        }
        if (forceServer) {
//            throw new URISyntaxException(host, "Illegal character in host name", 0);
            return E_URI_SYNTAX_EXCEPTION;
        }
        *isValid = FALSE;
        return NOERROR;
    }

    // IPv4 address
    AutoPtr<IInetAddress> is;
    InetAddress::ParseNumericAddress(host, (IInetAddress**)&is);
    if(is->Probe(EIID_IInet4Address) != NULL) {
        *isValid = TRUE;
        return NOERROR;
    }
    if (forceServer) {
//        throw new URISyntaxException(host, "Malformed IPv4 address", 0);
        return E_URI_SYNTAX_EXCEPTION;
    }

    *isValid = FALSE;
    return NOERROR;
}

Boolean CURI::IsValidDomainName(
    /* [in] */ const String& host)
{
    ECode ec = UriCodec::ValidateSimple(host, String("-."));
    if (ec == E_URI_SYNTAX_EXCEPTION) {
        return FALSE;
    }

    AutoPtr<ArrayOf<String> > values;
    StringUtils::Split(host, String("\\."), (ArrayOf<String>**)&values);
    Int32 count = values ? values->GetLength() : 0;
    String lastLabel;
    for (Int32 i = 0; i < count; ++i) {
        lastLabel = (*values)[i];
        if (lastLabel.StartWith("-") || lastLabel.EndWith("-")) {
            return FALSE;
        }
   }

    if (lastLabel.IsNull()) {
        return FALSE;
    }

    if (!lastLabel.Equals(host)) {
        Char32 ch = lastLabel.GetChar(0);
        if (ch >= '0' && ch <= '9') {
            return FALSE;
        }
    }
    return TRUE;
}

ECode CURI::CompareTo(
    /* [in] */ IURI* uri,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 ret;

    // compare schemes
    CURI* uriObj = (CURI*)uri;
    if (!mScheme.IsNull() || !uriObj->mScheme.IsNull()) {
        return mScheme.Compare(uriObj->mScheme);
    }

    // compare opacities
    if (!mOpaque && uriObj->mOpaque) {
        *result = -1;
        return NOERROR;
    }
    else if (mOpaque && !uriObj->mOpaque) {
        *result = 1;
        return NOERROR;
    }
    else if (mOpaque && uriObj->mOpaque) {
        ret = mSchemeSpecificPart.Compare(uriObj->mSchemeSpecificPart);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }
    }
    else {
        // otherwise both must be hierarchical

        // compare authorities
        if (!mAuthority.IsNull() && uriObj->mAuthority.IsNull()) {
            *result = 1;
            return NOERROR;
        }
        else if (mAuthority.IsNull() && !uriObj->mAuthority.IsNull()) {
            *result = -1;
            return NOERROR;
        }
        else if (!mAuthority.IsNull() && !uriObj->mAuthority.IsNull()) {
            if (!mHost.IsNull() && !uriObj->mHost.IsNull()) {
                // both are server based, so compare userInfo, host, port
                if (!mUserInfo.IsNull() && uriObj->mUserInfo.IsNull()) {
                    *result = 1;
                    return NOERROR;
                }
                else if (mUserInfo.IsNull() && !uriObj->mUserInfo.IsNull()) {
                    *result = -1;
                    return NOERROR;
                }
                else if (!mUserInfo.IsNull() && !uriObj->mUserInfo.IsNull()) {
                    ret = mUserInfo.Compare(uriObj->mUserInfo);
                    if (ret != 0) {
                        *result = ret;
                        return NOERROR;
                    }
                }

                // userInfo's are the same, compare hostname
                ret = mHost.CompareIgnoreCase(uriObj->mHost);
                if (ret != 0) {
                    *result = ret;
                    return NOERROR;
                }

                // compare port
                if (mPort != uriObj->mPort) {
                    return mPort - uriObj->mPort;
                }
            }
            else { // one or both are registry based, compare the whole
                // authority
                ret = mAuthority.Compare(uriObj->mAuthority);
                if (ret != 0) {
                    *result = ret;
                    return NOERROR;
                }
            }
        }

        // authorities are the same
        // compare paths
        ret = mPath.Compare(uriObj->mPath);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }

        // compare queries

        if (!mQuery.IsNull() && uriObj->mQuery.IsNull()) {
            *result = 1;
            return NOERROR;
        }
        else if (mQuery.IsNull() && !uriObj->mQuery.IsNull()) {
            *result = -1;
            return NOERROR;
        }
        else if (!mQuery.IsNull() && !uriObj->mQuery.IsNull()) {
            ret = mQuery.Compare(uriObj->mQuery);
            if (ret != 0) {
                *result = ret;
                return NOERROR;
            }
        }
    }

    // everything else is identical, so compare fragments
    if (!mFragment.IsNull() && uriObj->mFragment.IsNull()) {
        *result = 1;
        return NOERROR;
    }
    else if (mFragment.IsNull() && !uriObj->mFragment.IsNull()) {
        *result = -1;
        return NOERROR;
    }
    else if (!mFragment.IsNull() && !uriObj->mFragment.IsNull()) {
        ret = mFragment.Compare(uriObj->mFragment);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }
    }

    // identical
    *result = 0;
    return NOERROR;
}

ECode CURI::Create(
    /* [in] */ const String& uri,
    /* [out] */ IURI** obj)
{
    VALIDATE_NOT_NULL(obj);

//    try {
    AutoPtr<CURI> outuri;
    ECode ec = CURI::NewByFriend(uri, (CURI**)&outuri);
    if (ec == E_URI_SYNTAX_EXCEPTION) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *obj = (IURI*)outuri.Get();
    INTERFACE_ADDREF(*obj)
    return ec;
//    } catch (URISyntaxException e) {
//        throw new IllegalArgumentException(e.getMessage());
//    }
}

AutoPtr<CURI> CURI::Duplicate()
{
    AutoPtr<CURI> clone;
    CURI::NewByFriend((CURI**)&clone);
    clone->mAbsolute = mAbsolute;
    clone->mAuthority = mAuthority;
    clone->mFragment = mFragment;
    clone->mHost = mHost;
    clone->mOpaque = mOpaque;
    clone->mPath = mPath;
    clone->mPort = mPort;
    clone->mQuery = mQuery;
    clone->mScheme = mScheme;
    clone->mSchemeSpecificPart = mSchemeSpecificPart;
    clone->mUserInfo = mUserInfo;
    clone->mServerAuthority = mServerAuthority;
    return clone;
}

String CURI::ConvertHexToLowerCase(
    /* [in] */ const String& s)
{
    StringBuilder result;
    if (s.IndexOf('%') == -1) {
        return s;
    }

    Int32 index, prevIndex = 0;
    while ((index = s.IndexOf('%', prevIndex)) != -1) {
        result += s.Substring(prevIndex, index + 1);
        String temp;
        temp = s.Substring(index + 1, index + 3);
        result += temp.ToLowerCase();
        index += 3;
        prevIndex = index;
    }
    String resultStr;
    result.ToString(&resultStr);
    return resultStr;
}

Boolean CURI::EscapedEquals(
    /* [in] */ const String& first,
    /* [in] */ const String& second)
{
    if (first.IndexOf('%') != second.IndexOf('%')) {
        return first.Equals(second);
    }

    Int32 index, prevIndex = 0;
    while ((index = first.IndexOf('%', prevIndex)) != -1
            && second.IndexOf('%', prevIndex) == index) {
        Boolean match = first.Substring(prevIndex, index).Equals(
                second.Substring(prevIndex, index));
        if (!match) {
            return FALSE;
        }

        match = first.Substring(index + 1, index + 3).EqualsIgnoreCase(
                second.Substring(index + 1, index + 3));
        if (!match) {
            return FALSE;
        }

        index += 3;
        prevIndex = index;
    }
    return first.Substring(prevIndex).Equals(second.Substring(prevIndex));
}

ECode CURI::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    if (mHash == -1) {
        String hashString = GetHashString();
        mHash = hashString.GetHashCode();
    }

    *hash = mHash;
    return NOERROR;
}

//todo:implement the function of comparable
ECode CURI::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (IURI::Probe(o) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    CURI* uri = (CURI*)IURI::Probe(o);

    if ((uri->mFragment.IsNull() && !mFragment.IsNull())
        || (!uri->mFragment.IsNull() && mFragment.IsNull())) {
        *result = FALSE;
        return NOERROR;
    }
    else if (!uri->mFragment.IsNull() && !mFragment.IsNull()) {
        if (!EscapedEquals(uri->mFragment, mFragment)) {
            *result = FALSE;
            return NOERROR;
        }
    }

    if ((uri->mScheme.IsNull() && !mScheme.IsNull())
        || (!uri->mScheme.IsNull() && mScheme.IsNull())) {
        *result = FALSE;
        return NOERROR;
    }
    else if (!uri->mScheme.IsNull() && !mScheme.IsNull()) {
        if (!uri->mScheme.EqualsIgnoreCase(mScheme)) {
            *result = FALSE;
            return NOERROR;
        }
    }

    if (uri->mOpaque && mOpaque) {
        *result = EscapedEquals(uri->mSchemeSpecificPart,
                mSchemeSpecificPart);
        return NOERROR;
    }
    else if (!uri->mOpaque && !mOpaque) {
        if (!EscapedEquals(mPath, uri->mPath)) {
            *result = FALSE;
            return NOERROR;
        }

        if ((!uri->mQuery.IsNull() && mQuery.IsNull())
            || (uri->mQuery.IsNull() && !mQuery.IsNull())) {
            *result = FALSE;
            return NOERROR;
        }
        else if (!uri->mQuery.IsNull() && !mQuery.IsNull()) {
            if (!EscapedEquals(uri->mQuery, mQuery)) {
                *result = FALSE;
                return NOERROR;
            }
        }

        if ((!uri->mAuthority.IsNull() && mAuthority.IsNull())
            || (uri->mAuthority.IsNull() && !mAuthority.IsNull())) {
            *result = FALSE;
            return NOERROR;
        }
        else if (!uri->mAuthority.IsNull() && !mAuthority.IsNull()) {
            if ((!uri->mHost.IsNull() && mHost.IsNull())
                || (uri->mHost.IsNull() && !mHost.IsNull())) {
                *result = FALSE;
                return NOERROR;
            }
            else if (uri->mHost.IsNull() && mHost.IsNull()) {
                // both are registry based, so compare the whole authority
                *result = EscapedEquals(uri->mAuthority, mAuthority);
                return NOERROR;
            }
            else { // uri.host != null && host != null, so server-based
                if (!mHost.EqualsIgnoreCase(uri->mHost)) {
                    *result = FALSE;
                    return NOERROR;
                }

                if (mPort != uri->mPort) {
                    *result = FALSE;
                    return NOERROR;
                }

                if ((!uri->mUserInfo.IsNull() && mUserInfo.IsNull())
                    || (uri->mUserInfo.IsNull() && !mUserInfo.IsNull())) {
                    *result = FALSE;
                    return NOERROR;
                }
                else if (!uri->mUserInfo.IsNull() && !mUserInfo.IsNull()) {
                    *result = EscapedEquals(mUserInfo, uri->mUserInfo);
                    return NOERROR;
                }
                else {
                    *result = TRUE;
                    return NOERROR;
                }
            }
        }
        else {
            // no authority
            *result = TRUE;
            return NOERROR;
        }
    }
    else {
        // one is opaque, the other hierarchical
        *result = FALSE;
        return NOERROR;
    }
}

ECode CURI::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = mScheme;

    return NOERROR;
}

ECode CURI::GetSchemeSpecificPart(
    /* [out] */ String* schemeSpecific)
{
    VALIDATE_NOT_NULL(schemeSpecific);
    return Decode(mSchemeSpecificPart, schemeSpecific);
}

ECode CURI::GetRawSchemeSpecificPart(
    /* [out] */ String* schemeSpecific)
{
    VALIDATE_NOT_NULL(schemeSpecific);
    *schemeSpecific = mSchemeSpecificPart;

    return NOERROR;
}

ECode CURI::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    return Decode(mAuthority, authority);
}

ECode CURI::GetRawAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;

    return NOERROR;
}

ECode CURI::GetUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    return Decode(mUserInfo, userInfo);
}

ECode CURI::GetRawUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = mUserInfo;

    return NOERROR;
}

ECode CURI::GetHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);
    *host = mHost;

    return NOERROR;
}

ECode CURI::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;

    return NOERROR;
}

ECode CURI::GetEffectivePort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = GetEffectivePort(mScheme, mPort);

    return NOERROR;
}

Int32 CURI::GetEffectivePort(
    /* [in] */ const String& scheme,
    /* [in] */ Int32 specifiedPort)
{
    if (specifiedPort != -1) {
        return specifiedPort;
    }

    if (CString("http").EqualsIgnoreCase(scheme)) {
        return 80;
    }
    else if (CString("https").EqualsIgnoreCase(scheme)) {
        return 443;
    }
    else {
        return -1;
    }
}

ECode CURI::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    return Decode(mPath, path);
}

ECode CURI::GetRawPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath;

    return NOERROR;
}

ECode CURI::GetQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    return Decode(mQuery, query);
}

ECode CURI::GetRawQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = mQuery;

    return NOERROR;
}

ECode CURI::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    return Decode(mFragment, fragment);
}

ECode CURI::GetRawFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = mFragment;

    return NOERROR;
}

ECode CURI::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    VALIDATE_NOT_NULL(isAbsolute);
    *isAbsolute = mAbsolute;

    return NOERROR;
}

ECode CURI::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);
    *isOpaque = mOpaque;

    return NOERROR;
}

String CURI::Normalize(
    /* [in] */ const String& path,
    /* [in] */ Boolean discardRelativePrefix)
{
    String result = UrlUtils::CanonicalizePath(path, discardRelativePrefix);

    /*
     * If the path contains a colon before the first colon, prepend
     * "./" to differentiate the path from a scheme prefix.
     */
    Int32 colon = result.IndexOf(':');
    if (colon != -1) {
        Int32 slash = result.IndexOf('/');
        if (slash == -1 || colon < slash) {
            result = String("./") + result;
        }
    }

    return result;
}

ECode CURI::Normalize(
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)

    if (mOpaque) {
        *uri = (IURI*)this;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }
    String normalizedPath = Normalize(mPath, FALSE);
    // if the path is already normalized, return this
    if (mPath.Equals(normalizedPath)) {
        *uri = (IURI*)this;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }
    // get an exact copy of the URI re-calculate the scheme specific part
    // since the path of the normalized URI is different from this URI.
    AutoPtr<IURI> result = Duplicate();
    ((CURI*)result.Get())->mPath = normalizedPath;
    ((CURI*)result.Get())->SetSchemeSpecificPart();
    *uri = result;
    INTERFACE_ADDREF(*uri);
    return NOERROR;
}

ECode CURI::ParseServerAuthority(
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)

    if (!mServerAuthority) {
        ParseAuthority(TRUE);
    }
    *uri = (CURI*)this;
    INTERFACE_ADDREF(*uri);
    return NOERROR;
}

ECode CURI::Relativize(
    /* [in] */ IURI* relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    CURI* relativeObj = (CURI*)relative;
    if (relativeObj->mOpaque || mOpaque) {
        *uri = relative;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }

    if (mScheme.IsNull() ? !relativeObj->mScheme.IsNull() : !mScheme
            .Equals(relativeObj->mScheme)) {
        *uri = relative;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }

    if (mAuthority.IsNull() ? !relativeObj->mAuthority.IsNull() : !mAuthority
            .Equals(relativeObj->mAuthority)) {
        *uri = relative;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }

    // normalize both paths
    String thisPath = Normalize(mPath, FALSE);
    String relativePath = Normalize(relativeObj->mPath, FALSE);

    /*
     * if the paths aren't equal, then we need to determine if this URI's
     * path is a parent path (begins with) the relative URI's path
     */
    if (!thisPath.Equals(relativePath)) {
        // if this URI's path doesn't end in a '/', add one
        thisPath = thisPath.Substring(0, thisPath.LastIndexOf('/') + 1);
        /*
         * if the relative URI's path doesn't start with this URI's path,
         * then just return the relative URI; the URIs have nothing in
         * common
         */
        if (!relativePath.StartWith(thisPath)) {
            *uri = relative;
            INTERFACE_ADDREF(*uri);
            return NOERROR;
        }
    }

    AutoPtr<CURI> result;
    CURI::NewByFriend((CURI**)&result);
    result->mFragment = relativeObj->mFragment;
    result->mQuery = relativeObj->mQuery;
    // the result URI is the remainder of the relative URI's path
    result->mPath = relativePath.Substring(thisPath.GetLength());
    result->SetSchemeSpecificPart();
    *uri = (IURI*)result.Get();
    INTERFACE_ADDREF(*uri);

    return NOERROR;
}

ECode CURI::Resolve(
    /* [in] */ IURI* relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    CURI* relativeObj = (CURI*)relative;
    if (relativeObj->mAbsolute || mOpaque) {
        *uri = relative;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }

    if (relativeObj->mAuthority != NULL) {
        // If the relative URI has an authority, the result is the relative
        // with this URI's scheme.
        AutoPtr<IURI> result = relativeObj->Duplicate();
        CURI* cResult = (CURI*)result.Get();
        cResult->mScheme = mScheme;
        cResult->mAbsolute = mAbsolute;
        *uri = result;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }

    if ((relativeObj->mPath).IsEmpty() && relativeObj->mScheme == NULL && relativeObj->mQuery == NULL) {
        // if the relative URI only consists of at most a fragment,
        AutoPtr<IURI> result = Duplicate();
        CURI* cResult = (CURI*)result.Get();
        cResult->mFragment = relativeObj->mFragment;
        *uri = result;
        INTERFACE_ADDREF(*uri);
        return NOERROR;
    }

    AutoPtr<IURI> result = Duplicate();
    CURI* cResult = (CURI*)result.Get();
    cResult->mFragment = relativeObj->mFragment;
    cResult->mQuery = relativeObj->mQuery;
    String resolvedPath;
    if (relativeObj->mPath.StartWith("/")) {
        // The relative URI has an absolute path; use it.
        resolvedPath = relativeObj->mPath;
    } else if ((relativeObj->mPath).IsEmpty()) {
        // The relative URI has no path; use the base path.
        resolvedPath = mPath;
    } else {
        // The relative URI has a relative path; combine the paths.
        Int32 endIndex = mPath.LastIndexOf('/') + 1;
        resolvedPath = mPath.Substring(0, endIndex) + relativeObj->mPath;
    }
    cResult->mPath = UrlUtils::AuthoritySafePath(cResult->mAuthority, Normalize(resolvedPath, TRUE));
    cResult->SetSchemeSpecificPart();
    *uri = result;
    INTERFACE_ADDREF(*uri);
    return NOERROR;
}

void CURI::SetSchemeSpecificPart()
{
    // ssp = [//authority][path][?query]
    StringBuilder ssp;
    if (!mAuthority.IsNull()) {
        ssp += String("//") + mAuthority;
    }
    if (!mPath.IsNull()) {
        ssp += mPath;
    }
    if (!mQuery.IsNull()) {
        ssp += String("?") + mQuery;
    }
    ssp.ToString(&mSchemeSpecificPart);
    // reset string, so that it can be re-calculated correctly when asked.
    mString = NULL;
}

ECode CURI::ResolveEx(
    /* [in] */ const String& relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IURI> relativeURI;
    FAIL_RETURN(Create(relative, (IURI**)&relativeURI));
    return Resolve(relativeURI, uri);
}

ECode CURI::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* decodedS)
{
    VALIDATE_NOT_NULL(decodedS);
    if (s.IsNull()) {
        *decodedS = s;
    }else{
        FAIL_RETURN(UriCodec::Decode(s, decodedS));
    }
    return NOERROR;
}

ECode CURI::ToASCIIString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder result;
    String toString;
    ToString(&toString);
    FAIL_RETURN(ASCII_ONLY.AppendEncoded(result, toString));
    result.ToString(str);
    return NOERROR;
}

ECode CURI::ToString(
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s);

    if (!mString.IsNull()) {
        *s = mString;
        return NOERROR;
    }

    StringBuilder result;
    if (!mScheme.IsNull()) {
        result += mScheme;
        result += ':';
    }
    if (mOpaque) {
        result += mSchemeSpecificPart;
    }
    else {
        if (!mAuthority.IsNull()) {
            result += "//";
            result += mAuthority;
        }

        if (!mPath.IsNull()) {
            result += mPath;
        }

        if (!mQuery.IsNull()) {
            result += '?';
            result += mQuery;
        }
    }

    if (!mFragment.IsNull()) {
        result += '#';
        result += mFragment;
    }

    result.ToString(&mString);
    *s = mString;
    return NOERROR;
}

String CURI::GetHashString()
{
    StringBuilder result;
    if (!mScheme.IsNull()) {
        String temp = mScheme.ToLowerCase();
        result += temp;
        result += ':';
    }
    if (mOpaque) {
        result += mSchemeSpecificPart;
    }
    else {
        if (!mAuthority.IsNull()) {
            result += "//";
            if (mHost.IsNull()) {
                result += mAuthority;
            }
            else {
                if (!mUserInfo.IsNull()) {
                    result += mUserInfo + "@";
                }
                String temp = mHost.ToLowerCase();
                result += temp;
                if (mPort != -1) {
                    result += ":" + mPort;
                }
            }
        }

        if (!mPath.IsNull()) {
            result += mPath;
        }

        if (!mQuery.IsNull()) {
            result += '?';
            result += mQuery;
        }
    }

    if (!mFragment.IsNull()) {
        result += '#';
        result += mFragment;
    }

    String resultTemp;
    result.ToString(&resultTemp);
    return ConvertHexToLowerCase(resultTemp);
}

ECode CURI::ToURL(
    /* [out] */ IURL** url)
{
    VALIDATE_NOT_NULL(url);

    if (!mAbsolute) {
//        throw new IllegalArgumentException("URI is not absolute: " + toString());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String s;
    ToString(&s);
    return CURL::New(s, url);
}

} // namespace Net
} // namespace Elastos
