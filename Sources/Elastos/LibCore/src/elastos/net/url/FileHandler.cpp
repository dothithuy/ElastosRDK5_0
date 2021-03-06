
#include "cmdef.h"
#include "FileHandler.h"
#include "CURL.h"
#include "CFileURLConnection.h"

namespace Elastos {
namespace Net {
namespace Url {

ECode FileHandler::OpenConnection(
    /* [in] */ IURL* u,
    /* [out] */ IURLConnection** urlConnection)
{
    return OpenConnectionEx(u, NULL, urlConnection);
}

ECode FileHandler::OpenConnectionEx(
    /* [in] */ IURL* u,
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** urlConnection)
{
    VALIDATE_NOT_NULL(urlConnection)

    if (u == NULL) {
        // throw new IllegalArgumentException("url == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String host;
    u->GetHost(&host);
    if (host.IsNull() || host.IsEmpty() || host.EqualsIgnoreCase("localhost")) {
        AutoPtr<IFileURLConnection> furl;
        CFileURLConnection::New(u, (IFileURLConnection**)&furl);
        *urlConnection = furl;
        INTERFACE_ADDREF(*urlConnection)
        return NOERROR;
    }

    // If a hostname is specified try to get the resource using FTP
    String filename;
    AutoPtr<IURL> ftpURL;
    FAIL_RETURN(CURL::New(String("ftp"), host, (u->GetFile(&filename), filename), (IURL**)&ftpURL));
    AutoPtr<IURLConnection> outurl;
    *urlConnection = (proxy == NULL)
                    ? (ftpURL->OpenConnection((IURLConnection**)&outurl), outurl)
                    : (ftpURL->OpenConnectionEx(proxy, (IURLConnection**)&outurl), outurl);
    INTERFACE_ADDREF(*urlConnection)

    return NOERROR;
}

ECode FileHandler::ParseURL(
    /* [in] */ IURL* url,
    /* [in] */ const String& spec,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (end < start) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String parseString = String("");
    if (start < end) {
        parseString = spec.Substring(start, end).Replace('\\', '/');
    }
    return URLStreamHandler::ParseURL(url, parseString, 0, parseString.GetLength());
}

} // namespace Url
} // namespace Net
} // namespace Elastos
