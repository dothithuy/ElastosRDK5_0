
#include "cmdef.h"
#include "FtpURLConnection.h"
#include "ProxySelector.h"
#include "elastos/List.h"
#include "elastos/StringBuilder.h"
#include "CProxy.h"
#include "CSocket.h"
#include "CInetSocketAddress.h"
#include "CServerSocket.h"
#include "CBufferedInputStream.h"
#include "StringUtils.h"
#include "CFtpURLInputStream.h"

using Elastos::Utility::List;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Net {
namespace Url {

const Int32 FtpURLConnection::FTP_PORT = 21;

// FTP Reply Constants
const Int32 FtpURLConnection::FTP_DATAOPEN = 125;

const Int32 FtpURLConnection::FTP_OPENDATA = 150;

const Int32 FtpURLConnection::FTP_OK = 200;

const Int32 FtpURLConnection::FTP_USERREADY = 220;

const Int32 FtpURLConnection::FTP_TRANSFEROK = 226;

const Int32 FtpURLConnection::FTP_LOGGEDIN = 230;

const Int32 FtpURLConnection::FTP_FILEOK = 250;

const Int32 FtpURLConnection::FTP_PASWD = 331;

const Int32 FtpURLConnection::FTP_NOTFOUND = 550;

CAR_INTERFACE_IMPL(FtpURLConnection, IFtpURLConnection)

ECode FtpURLConnection::Connect()
{
    // Use system-wide ProxySelect to select proxy list,
    // then try to connect via elements in the proxy list.
    List< AutoPtr<IProxy> > proxyList;
    if (mProxy != NULL) {
        proxyList.PushBack(mProxy);
    }
    else {
        AutoPtr<IProxySelector> selector;
        ProxySelector::GetDefault((IProxySelector**)&selector);
        if (selector != NULL) {
            AutoPtr<IObjectContainer> obj;
            selector->Select(mUri, (IObjectContainer**)&obj);
            AutoPtr<IObjectEnumerator> emu;
            obj->GetObjectEnumerator((IObjectEnumerator **)&emu);
            Boolean hasNext = FALSE;
            while(emu->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> itf;
                emu->Current((IInterface **)&itf);
                AutoPtr<IProxy> entry = IProxy::Probe(itf);
                proxyList.PushBack(entry);
            }
        }
    }
    if (proxyList.IsEmpty()) {
        mCurrentProxy = NULL;
        ConnectInternal();
    }
    else {
        AutoPtr<IProxySelector> selector;
        ProxySelector::GetDefault((IProxySelector**)&selector);
        List< AutoPtr<IProxy> >::Iterator iter = proxyList.Begin();
        Boolean connectOK = FALSE;
        String failureReason = String("");
        while ((++iter, iter != proxyList.End()) && !connectOK) {
            mCurrentProxy = *iter;
            // try {
            ECode ec = ConnectInternal();
            connectOK = TRUE;
            // } catch (IOException ioe) {
            if (ec != NOERROR) {
                // failureReason = ioe.getLocalizedMessage();
                // If connect failed, callback "connectFailed"
                // should be invoked.
                if (selector != NULL && CProxy::NO_PROXY != mCurrentProxy) {
                    AutoPtr<ISocketAddress> sockadd;
                    mCurrentProxy->GetAddress((ISocketAddress**)&sockadd);
                    selector->ConnectFailed(mUri, sockadd, ec);
                }
            }
            // }
        }
        if (!connectOK) {
            // throw new IOException("Unable to connect to server: " + failureReason);
            return E_IO_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode FtpURLConnection::GetContentType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type)

    String filename;
    mUrl->GetFile(&filename);
    String result = GuessContentTypeFromName(filename);
    if (result.IsNull()) {
        *type = String("content/unknown");
    }
    else {
        *type = result;
    }

    return NOERROR;
}

ECode FtpURLConnection::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is)

    if (!mConnected) {
        FAIL_RETURN(Connect());
    }
    *is = mInputStream;
    INTERFACE_ADDREF(*is)

    return NOERROR;
}

ECode FtpURLConnection::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os)

    if (!mConnected) {
        FAIL_RETURN(Connect());
    }
    AutoPtr<IOutputStream> output;
    mDataSocket->GetOutputStream((IOutputStream**)&output);
    *os = output;
    INTERFACE_ADDREF(*os)

    return NOERROR;
}

//        public java.security.Permission getPermission();

ECode FtpURLConnection::SetDoInput(
    /* [in] */ Boolean newValue)
{
    if (mConnected) {
        // throw new IllegalAccessError();
        return E_ILLEGAL_ACCESS_ERROR_EXCEPTION;
    }
    mDoInput = newValue;
    mDoOutput = !newValue;
    return NOERROR;
}

ECode FtpURLConnection::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    if (mConnected) {
        // throw new IllegalAccessError();
        return E_ILLEGAL_ACCESS_ERROR_EXCEPTION;
    }
    mDoOutput = newValue;
    mDoInput = !newValue;
    return NOERROR;
}

ECode FtpURLConnection::Init(
    /* [in] */ IURL* url)
{
    FAIL_RETURN(URLConnection::Init(url));
    mUsername = "anonymous";
    mPassword = "";
    mUrl->GetHost(&mHostName);
    String parse;
    mUrl->GetUserInfo(&parse);
    if (!parse.IsNull()) {
        Int32 split = parse.IndexOf(':');
        if (split >= 0) {
            mUsername = parse.Substring(0, split);
            mPassword = parse.Substring(split + 1);
        }
        else {
            mUsername = parse;
        }
    }
    mUri = NULL;
    // try {
    url->ToURI((IURI**)&mUri);
    // } catch (URISyntaxException e) {
    //     // do nothing.
    // }
    return NOERROR;
}

FtpURLConnection::FtpURLConnection(
    /* [in] */ IURL* url)
{
    Init(url);
}

FtpURLConnection::FtpURLConnection(
    /* [in] */ IURL* url,
    /* [in] */ IProxy* proxy)
{
    Init(url);
    mProxy = proxy;
}

ECode FtpURLConnection::Cd()
{
    String filename;
    mUrl->GetFile(&filename);

    Int32 idx = filename.LastIndexOf('/');

    if (idx > 0) {
        String dir = filename.Substring(0, idx);
        Write(String("CWD ") + dir + String("\r\n"));
        Int32 reply = 0;
        GetReply(&reply);
        if (reply != FTP_FILEOK && dir.GetLength() > 0 && dir.GetChar(0) == '/') {
            Write(String("CWD ") + dir.Substring(1) + String("\r\n"));
            GetReply(&reply);
        }
        if (reply != FTP_FILEOK) {
            // throw new IOException("Unable to change directories");
            return E_IO_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode FtpURLConnection::ConnectInternal()
{
    Int32 port = 0;
    mUrl->GetPort(&port);
    Int32 connectTimeout = 0;
    GetConnectTimeout(&connectTimeout);
    if (port <= 0) {
        port = FTP_PORT;
    }
    ProxyType typeflag;
    mControlSocket = NULL;
    if (mCurrentProxy == NULL || ProxyType_HTTP == (mCurrentProxy->GetType(&typeflag), typeflag)) {
        FAIL_RETURN(CSocket::New((ISocket**)&mControlSocket));
    }
    else {
        FAIL_RETURN(CSocket::New(mCurrentProxy, (ISocket**)&mControlSocket));
    }
    AutoPtr<IInetSocketAddress> addr;
    FAIL_RETURN(CInetSocketAddress::New(mHostName, port, (IInetSocketAddress**)&addr));
    mControlSocket->ConnectEx(addr, connectTimeout);
    mConnected = TRUE;
    mCtrlOutput = NULL;
    mControlSocket->GetOutputStream((IOutputStream**)&mCtrlOutput);
    mCtrlInput = NULL;
    mControlSocket->GetInputStream((IInputStream**)&mCtrlInput);
    Login();
    SetType();
    Boolean isflag = FALSE;
    if (!(GetDoInput(&isflag), isflag)) {
        Cd();
    }

    // try {
        mAcceptSocket = NULL;
        CServerSocket::New(0, (IServerSocket**)&mAcceptSocket);
        mAcceptSocket->GetLocalPort(&mDataPort);
        // Cannot set REUSEADDR so we need to send a PORT command
        Port();
        if (connectTimeout == 0) {
            // set timeout rather than zero as before
            connectTimeout = 3000;
        }
        mAcceptSocket->SetSoTimeout((GetConnectTimeout(&connectTimeout), connectTimeout));
        if (GetDoInput(&isflag), isflag) {
            GetFile();
        }
        else {
            SendFile();
        }
        mDataSocket = NULL;
        mAcceptSocket->Accept((ISocket**)&mDataSocket);
        Int32 readtime = 0;
        mDataSocket->SetSoTimeout((GetReadTimeout(&readtime), readtime));
        mAcceptSocket->Close();
    // } catch (InterruptedIOException e) {
    //     throw new IOException("Could not establish data connection");
    // }
    if (GetDoInput(&isflag), isflag) {
        AutoPtr<IInputStream> datainput;
        mDataSocket->GetInputStream((IInputStream**)&datainput);
        AutoPtr<IBufferedInputStream> bis;
        CBufferedInputStream::New(datainput, (IBufferedInputStream**)&bis);
        mInputStream = NULL;
        CFtpURLInputStream::New(bis, mControlSocket, (IFtpURLInputStream**)&mInputStream);
    }
    return NOERROR;
}

ECode FtpURLConnection::GetFile()
{
    Int32 reply = 0;
    String file;
    mUrl->GetFile(&file);
    Write(String("RETR ") + file + String("\r\n"));
    GetReply(&reply);
    if (reply == FTP_NOTFOUND && file.GetLength() > 0 && file.GetChar(0) == '/') {
        Write(String("RETR ") + file.Substring(1) + String("\r\n"));
        GetReply(&reply);
    }
    if (!(reply == FTP_OPENDATA || reply == FTP_TRANSFEROK)) {
        // throw new FileNotFoundException("Unable to retrieve file: " + reply);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
}

ECode FtpURLConnection::GetReply(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr< ArrayOf<Char32> > code = ArrayOf<Char32>::Alloc(3);
    for (Int32 i = 0; i < code->GetLength(); i++) {
        Int32 tmp = 0;
        mCtrlInput->Read(&tmp);
        if (tmp == -1) {
            // throw new EOFException();
            return E_IO_EXCEPTION;
        }
        (*code)[i] = tmp;
    }
    mReplyCode = String(*code, 0, code->GetLength());

    Boolean multiline = FALSE;
    Int32 readint = 0;
    if ((mCtrlInput->Read(&readint), readint) == '-') {
        multiline = TRUE;
    }
    ReadLine(); /* Skip the rest of the first line */
    if (multiline) {
        while (ReadMultiLine()) {/* Read all of a multiline reply */
        }
    }

    // try {
    *value = StringUtils::ParseInt32(mReplyCode);
    // } catch (NumberFormatException e) {
    //     throw (IOException)(new IOException("reply code is invalid").initCause(e));
    // }
    return NOERROR;
}

ECode FtpURLConnection::Login()
{
    Int32 reply = 0;
    GetReply(&reply);
    if (reply == FTP_USERREADY) {
    }
    else {
        // throw new IOException("Unable to connect to server: " + url.getHost());
        return E_IO_EXCEPTION;
    }
    Write(String("USER ") + mUsername + String("\r\n"));
    GetReply(&reply);
    if (reply == FTP_PASWD || reply == FTP_LOGGEDIN) {
    }
    else {
        // throw new IOException("Unable to log in to server (USER): " + url.getHost());
        return E_IO_EXCEPTION;
    }
    if (reply == FTP_PASWD) {
        Write(String("PASS ") + mPassword + String("\r\n"));
        GetReply(&reply);
        if (!(reply == FTP_OK || reply == FTP_USERREADY || reply == FTP_LOGGEDIN)) {
            // throw new IOException("Unable to log in to server (PASS): " + url.getHost());
            return E_IO_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode FtpURLConnection::Port()
{
    AutoPtr<IInetAddress> iadd;
    mControlSocket->GetLocalAddress((IInetAddress**)&iadd);
    String outstr;
    iadd->GetHostAddress(&outstr);
    String inputstr = String("PORT ") + outstr.Replace('.',',');
    inputstr.Append(',');
    inputstr += StringUtils::Int32ToString(mDataPort >> 8);
    inputstr.Append(',');
    inputstr += StringUtils::Int32ToString(mDataPort & 255) + String("\r\n");
    Write(inputstr);

    Int32 value = 0;
    if ((GetReply(&value), value) != FTP_OK) {
        // throw new IOException("Unable to configure data port");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

String FtpURLConnection::ReadLine()
{
    StringBuilder sb;
    Int32 c = 0;
    while ((mCtrlInput->Read(&c), c) != '\n') {
        sb.AppendChar((Char32) c);
    }
    return sb.ToString();
}

Boolean FtpURLConnection::ReadMultiLine()
{
    String line = ReadLine();
    if (line.GetLength() < 4) {
        return TRUE;
    }
    if (line.Substring(0, 3).Equals(mReplyCode)
            && (line.GetChar(3) == (Char32) 32)) {
        return FALSE;
    }
    return TRUE;
}

ECode FtpURLConnection::SendFile()
{
    Int32 reply = 0;
    String filename;
    mUrl->GetFile(&filename);
    Write(String("STOR ")
            + filename.Substring(filename.LastIndexOf('/') + 1, filename.GetLength())
            + String("\r\n"));
    GetReply(&reply);
    if (!(reply == FTP_OPENDATA || reply == FTP_OK || reply == FTP_DATAOPEN)) {
        // throw new IOException("Unable to store file");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode FtpURLConnection::SetType()
{
    Write(String("TYPE I\r\n"));
    Int32 reply = 0;
    if ((GetReply(&reply), reply) != FTP_OK) {
        // throw new IOException("Unable to set transfer type");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

void FtpURLConnection::Write(
    /* [in] */ const String& command)
{
    AutoPtr<ArrayOf<Char32> > buf32 = command.GetChars();
    Int32 len = buf32->GetLength();
    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(len);
    for (Int32 i = 0; i < len; ++i) {
        (*buffer)[i] = (*buf32)[i] && 0xFF;
    }
    mCtrlOutput->WriteBytes(*buffer);
}

} // namespace Url
} // namespace Net
} // namespace Elastos
