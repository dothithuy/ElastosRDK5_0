#include "ext/frameworkdef.h"
#include "CWifiP2pManagerChannel.h"
#include <elastos/Slogger.h>

using Elastos::Droid::Os::IBundle;
using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Net::Wifi::P2p::Nsd::IWifiP2pDnsSdServiceInfo;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

const Int32 CWifiP2pManagerChannel::INVALID_LISTENER_KEY = 0;

//=========================================================================
// CWifiP2pManagerChannel::P2pHandler
//=========================================================================
CWifiP2pManagerChannel::P2pHandler::P2pHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CWifiP2pManagerChannel* host)
    : HandlerBase(looper)
    , mHost(host)
{
}

ECode CWifiP2pManagerChannel::P2pHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    AutoPtr<IInterface> obj;
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    msg->GetObj((IInterface**)&obj);

    AutoPtr<IInterface> listener;
    mHost->GetListener(arg2, (IInterface**)&listener);
    switch (what) {
        case AsyncChannel::CMD_CHANNEL_DISCONNECTED:
            if (mHost->mChannelListener != NULL) {
                mHost->mChannelListener->OnChannelDisconnected();
                mHost->mChannelListener = NULL;
            }
            break;
        /* ActionListeners grouped together */
        case IWifiP2pManager::DISCOVER_PEERS_FAILED:
        case IWifiP2pManager::STOP_DISCOVERY_FAILED:
        case IWifiP2pManager::DISCOVER_SERVICES_FAILED:
        case IWifiP2pManager::CONNECT_FAILED:
        case IWifiP2pManager::CANCEL_CONNECT_FAILED:
        case IWifiP2pManager::CREATE_GROUP_FAILED:
        case IWifiP2pManager::REMOVE_GROUP_FAILED:
        case IWifiP2pManager::ADD_LOCAL_SERVICE_FAILED:
        case IWifiP2pManager::REMOVE_LOCAL_SERVICE_FAILED:
        case IWifiP2pManager::CLEAR_LOCAL_SERVICES_FAILED:
        case IWifiP2pManager::ADD_SERVICE_REQUEST_FAILED:
        case IWifiP2pManager::REMOVE_SERVICE_REQUEST_FAILED:
        case IWifiP2pManager::CLEAR_SERVICE_REQUESTS_FAILED:
        case IWifiP2pManager::SET_DEVICE_NAME_FAILED:
        case IWifiP2pManager::DELETE_PERSISTENT_GROUP_FAILED:
        case IWifiP2pManager::SET_WFD_INFO_FAILED:
        case IWifiP2pManager::SET_GO_PSK_FAILED:
            if (listener != NULL) {
                AutoPtr<IWifiP2pManagerActionListener> al;
                al = IWifiP2pManagerActionListener::Probe(listener);
                al->OnFailure(arg1);
            }
            break;
        /* ActionListeners grouped together */
        case IWifiP2pManager::DISCOVER_PEERS_SUCCEEDED:
        case IWifiP2pManager::STOP_DISCOVERY_SUCCEEDED:
        case IWifiP2pManager::DISCOVER_SERVICES_SUCCEEDED:
        case IWifiP2pManager::CONNECT_SUCCEEDED:
        case IWifiP2pManager::CANCEL_CONNECT_SUCCEEDED:
        case IWifiP2pManager::CREATE_GROUP_SUCCEEDED:
        case IWifiP2pManager::REMOVE_GROUP_SUCCEEDED:
        case IWifiP2pManager::ADD_LOCAL_SERVICE_SUCCEEDED:
        case IWifiP2pManager::REMOVE_LOCAL_SERVICE_SUCCEEDED:
        case IWifiP2pManager::CLEAR_LOCAL_SERVICES_SUCCEEDED:
        case IWifiP2pManager::ADD_SERVICE_REQUEST_SUCCEEDED:
        case IWifiP2pManager::REMOVE_SERVICE_REQUEST_SUCCEEDED:
        case IWifiP2pManager::CLEAR_SERVICE_REQUESTS_SUCCEEDED:
        case IWifiP2pManager::SET_DEVICE_NAME_SUCCEEDED:
        case IWifiP2pManager::DELETE_PERSISTENT_GROUP_SUCCEEDED:
        case IWifiP2pManager::SET_WFD_INFO_SUCCEEDED:
        case IWifiP2pManager::SET_GO_PSK_SUCCEEDED:
            if (listener != NULL) {
                AutoPtr<IWifiP2pManagerActionListener> al;
                al = IWifiP2pManagerActionListener::Probe(listener);
                assert(al != NULL);
                al->OnSuccess();
            }
            break;
        case IWifiP2pManager::RESPONSE_PEERS: {
                AutoPtr<IWifiP2pDeviceList> peers = IWifiP2pDeviceList::Probe(obj);
                if (listener != NULL) {
                    AutoPtr<IWifiP2pManagerPeerListListener> al;
                    al = IWifiP2pManagerPeerListListener::Probe(listener);
                    assert(al != NULL);
                    al->OnPeersAvailable(peers);
                }
            }
            break;
        case IWifiP2pManager::RESPONSE_CONNECTION_INFO: {
                AutoPtr<IWifiP2pInfo> wifiP2pInfo = IWifiP2pInfo::Probe(obj);
                if (listener != NULL) {
                    AutoPtr<IWifiP2pManagerConnectionInfoListener> al;
                    al = IWifiP2pManagerConnectionInfoListener::Probe(listener);
                    assert(al != NULL);
                    al->OnConnectionInfoAvailable(wifiP2pInfo);
                }
            }
            break;
        case IWifiP2pManager::RESPONSE_GROUP_INFO: {
                AutoPtr<IWifiP2pGroup> group = IWifiP2pGroup::Probe(obj);
                if (listener != NULL) {
                    AutoPtr<IWifiP2pManagerGroupInfoListener> al;
                    al = IWifiP2pManagerGroupInfoListener::Probe(listener);
                    assert(al != NULL);
                    al->OnGroupInfoAvailable(group);
                }
            }
            break;
        case IWifiP2pManager::RESPONSE_SERVICE: {
                AutoPtr<IWifiP2pServiceResponse> resp = IWifiP2pServiceResponse::Probe(obj);
                mHost->HandleServiceResponse(resp);
            }
            break;
        case IWifiP2pManager::CONNECTION_REQUESTED:
            if (mHost->mDialogListener != NULL) {
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                AutoPtr<IParcelable> parcel;
                bundle->GetParcelable(IWifiP2pManager::P2P_DEV_BUNDLE_KEY, (IParcelable**)&parcel);
                AutoPtr<IWifiP2pDevice> device = IWifiP2pDevice::Probe(parcel);
                assert(device != NULL);
                parcel = NULL;
                bundle->GetParcelable(IWifiP2pManager::P2P_CONFIG_BUNDLE_KEY, (IParcelable**)&parcel);
                AutoPtr<IWifiP2pConfig> config = IWifiP2pConfig::Probe(parcel);
                assert(config != NULL);
                mHost->mDialogListener->OnConnectionRequested(device, config);
            }
            break;
        case IWifiP2pManager::SHOW_PIN_REQUESTED:
            if (mHost->mDialogListener != NULL) {
                AutoPtr<IBundle> bundle;
                msg->GetData((IBundle**)&bundle);
                String pin;
                bundle->GetString(IWifiP2pManager::WPS_PIN_BUNDLE_KEY, &pin);
                mHost->mDialogListener->OnShowPinRequested(pin);
            }
            break;
        case IWifiP2pManager::DIALOG_LISTENER_ATTACHED:
            if (mHost->mDialogListener != NULL) {
                mHost->mDialogListener->OnAttached();
            }
            break;
        case IWifiP2pManager::DIALOG_LISTENER_DETACHED:
            if (mHost->mDialogListener != NULL) {
                mHost->mDialogListener->OnDetached(arg1);
                mHost->mDialogListener = NULL;
            }
            break;
        case IWifiP2pManager::RESPONSE_PERSISTENT_GROUP_INFO: {
                AutoPtr<IWifiP2pGroupList> groups = IWifiP2pGroupList::Probe(obj);
                if (listener != NULL) {
                    AutoPtr<IWifiP2pManagerPersistentGroupInfoListener> al;
                    al = IWifiP2pManagerPersistentGroupInfoListener::Probe(listener);
                    assert(al != NULL);
                    al->OnPersistentGroupInfoAvailable(groups);
                }
            }
            break;
       default:
            Slogger::D("WifiP2pManagerChannel", "Ignored %d", what);
            break;
    }

    return NOERROR;
}

//=========================================================================
// CWifiP2pManagerChannel
//=========================================================================

CWifiP2pManagerChannel::CWifiP2pManagerChannel()
    : mListenerKey(0)
{
}

CWifiP2pManagerChannel::~CWifiP2pManagerChannel()
{
}

ECode CWifiP2pManagerChannel::constructor(
    /* [in] */ IContext *context,
    /* [in] */ ILooper * looper,
    /* [in] */ IWifiP2pManagerChannelListener* l)
{
    mAsyncChannel = new AsyncChannel();
    mHandler = new P2pHandler(looper, this);
    mChannelListener = l;
    mContext = context;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetHandler(
    /* [out] */ IHandler** handler)
{
    VALIDATE_NOT_NULL(handler);
    *handler = mHandler;
    INTERFACE_ADDREF(*handler);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetHandler(
    /* [in] */ IHandler* handler)
{
    mHandler = handler;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = mContext;
    INTERFACE_ADDREF(*context);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetContext(
        /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetChannelListener(
    /* [out] */ IWifiP2pManagerChannelListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mChannelListener;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetChannelListener(
    /* [in] */ IWifiP2pManagerChannelListener* listener)
{
    mChannelListener = listener;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetServRspListener(
    /* [out] */ IWifiP2pManagerServiceResponseListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mServRspListener;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetServRspListener(
    /* [in] */ IWifiP2pManagerServiceResponseListener* listener)
{
    mServRspListener = listener;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetDnsSdServRspListener(
    /* [out] */ IWifiP2pManagerDnsSdServiceResponseListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mDnsSdServRspListener;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetDnsSdServRspListener(
    /* [in] */ IWifiP2pManagerDnsSdServiceResponseListener* listener)
{
    mDnsSdServRspListener = listener;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetDnsSdTxtListener(
    /* [out] */ IWifiP2pManagerDnsSdTxtRecordListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mDnsSdTxtListener;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetDnsSdTxtListener(
    /* [in] */ IWifiP2pManagerDnsSdTxtRecordListener* listener)
{
    mDnsSdTxtListener = listener;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetUpnpServRspListener(
    /* [out] */ IWifiP2pManagerUpnpServiceResponseListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mUpnpServRspListener;
    INTERFACE_ADDREF(*listener);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetUpnpServRspListener(
    /* [in] */ IWifiP2pManagerUpnpServiceResponseListener* listener)
{
    mUpnpServRspListener = listener;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::HandleServiceResponse(
    /* [in] */ IWifiP2pServiceResponse* resp)
{
    if (IWifiP2pDnsSdServiceResponse::Probe(resp) != NULL) {
        HandleDnsSdServiceResponse(IWifiP2pDnsSdServiceResponse::Probe(resp));
    }
    else if (IWifiP2pUpnpServiceResponse::Probe(resp) != NULL) {
        if (mUpnpServRspListener != NULL) {
            HandleUpnpServiceResponse(IWifiP2pUpnpServiceResponse::Probe(resp));
        }
    }
    else {
        if (mServRspListener != NULL) {
            Int32 type;
            AutoPtr<ArrayOf<Byte> > data;
            AutoPtr<IWifiP2pDevice> device;
            resp->GetServiceType(&type);
            resp->GetRawData((ArrayOf<Byte>**)&data);
            resp->GetSrcDevice((IWifiP2pDevice**)&device);
            mServRspListener->OnServiceAvailable(type, data, device);
        }
    }
    return NOERROR;
}

ECode CWifiP2pManagerChannel::HandleUpnpServiceResponse(
    /* [in] */ IWifiP2pUpnpServiceResponse* resp)
{
    AutoPtr<ArrayOf<String> > names;
    AutoPtr<IWifiP2pDevice> device;
    resp->GetUniqueServiceNames((ArrayOf<String>**)&names);
    resp->GetSrcDevice((IWifiP2pDevice**)&device);
    mUpnpServRspListener->OnUpnpServiceAvailable(names, device);
    return NOERROR;
}

ECode CWifiP2pManagerChannel::HandleDnsSdServiceResponse(
    /* [in] */ IWifiP2pDnsSdServiceResponse* resp)
{
    VALIDATE_NOT_NULL(resp);

    Int32 type;
    resp->GetDnsType(&type);
    String instanceName, queryName;
    AutoPtr<IWifiP2pDevice> device;
    AutoPtr<IObjectStringMap> map;
    resp->GetInstanceName(&instanceName);
    resp->GetDnsQueryName(&queryName);
    resp->GetSrcDevice((IWifiP2pDevice**)&device);
    resp->GetTxtRecord((IObjectStringMap**)&map);

    if (type == IWifiP2pDnsSdServiceInfo::DNS_TYPE_PTR) {
        if (mDnsSdServRspListener != NULL) {
            mDnsSdServRspListener->OnDnsSdServiceAvailable(
                instanceName, queryName, device);
        }
    } else if (type == IWifiP2pDnsSdServiceInfo::DNS_TYPE_TXT) {
        if (mDnsSdTxtListener != NULL) {
            mDnsSdTxtListener->OnDnsSdTxtRecordAvailable(
                queryName, map, device);
        }
    }
    else {
        String info;
        resp->ToString(&info);
        Slogger::E("CWifiP2pManagerChannel", "Unhandled resp %s", info.string());
    }
    return NOERROR;
}

ECode CWifiP2pManagerChannel::PutListener(
    /* [in] */ IInterface* listener,
    /* [out] */ Int32* val)
{
    if (listener == NULL) {
        *val = INVALID_LISTENER_KEY;
        return NOERROR;
    }

    Int32 key;
    Mutex::Autolock lock(mListenerMapLock);
    do {
        key = mListenerKey++;
    } while (key == INVALID_LISTENER_KEY);
    typedef typename HashMap<Int32, AutoPtr<IInterface> >::ValueType PairValueType;
    mListenerMap.Insert(PairValueType(key, listener));
    *val = key;
    return NOERROR;
}

ECode CWifiP2pManagerChannel::GetListener(
    /* [in] */ Int32 key,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = NULL;

    if (key == INVALID_LISTENER_KEY) {
        return NOERROR;
    }

    Mutex::Autolock lock(mListenerMapLock);
    HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mListenerMap.Find(key);
    if (it != mListenerMap.End()) {
        *obj = it->mSecond;
        INTERFACE_ADDREF(*obj);
        mListenerMap.Erase(it);
    }

    return NOERROR;
}

ECode CWifiP2pManagerChannel::SetDialogListener(
    /* [in] */ IWifiP2pManagerDialogListener* listener)
{
    mDialogListener = listener;
    return NOERROR;
}

}
}
}
}
}