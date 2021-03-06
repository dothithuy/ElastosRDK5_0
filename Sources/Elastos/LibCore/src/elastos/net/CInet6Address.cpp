
#include "cmdef.h"
#include "CInet6Address.h"
#include "NetworkInterface.h"

namespace Elastos {
namespace Net {

static AutoPtr<IInetAddress> InitANY()
{
    AutoPtr<ArrayOf<Byte> > ipAddress = ArrayOf<Byte>::Alloc(16);
    memset(ipAddress->GetPayload(), 0, 16);
    AutoPtr<CInet6Address> ipv6Addr;
    CInet6Address::NewByFriend(ipAddress, String(NULL), 0, (CInet6Address**)&ipv6Addr);
    return (IInetAddress*)(IInet6Address*)ipv6Addr.Get();
}

static AutoPtr<IInetAddress> InitLOOPBACK()
{
    AutoPtr<ArrayOf<Byte> > ipAddress = ArrayOf<Byte>::Alloc(16);
    memset(ipAddress->GetPayload(), 0, 16);
    (*ipAddress)[15] = 1;
    AutoPtr<CInet6Address> ipv6Addr;
    CInet6Address::NewByFriend(ipAddress, String("localhost"), 0,
            (CInet6Address**)&ipv6Addr);
    return (IInetAddress*)(IInet6Address*)ipv6Addr.Get();
}

const Int32 CInet6Address::AF_INET6;
AutoPtr<IInetAddress> CInet6Address::ANY = InitANY();
AutoPtr<IInetAddress> CInet6Address::LOOPBACK = InitLOOPBACK();

CInet6Address::CInet6Address()
    : mScopeIdSet(FALSE)
    , mScopeIfnameSet(FALSE)
{}

CInet6Address::CInet6Address(
    /* [in] */ ArrayOf<Byte>* address,
    /* [in] */ const String& name,
    /* [in] */ Int32 scopeId)
    : InetAddress(AF_INET6, address, name)
{
    mScopeId = scopeId;
    mScopeIdSet = (scopeId != 0);
}
PInterface CInet6Address::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_InetAddress) {
        return reinterpret_cast<PInterface>((InetAddress*)this);
    }
    else return _CInet6Address::Probe(riid);
}

/**
 * Constructs an IPv6 address according to the given {@code host}, {@code
 * addr} and {@code scope_id}.
 *
 * @param host
 *            the host name associated with the address.
 * @param addr
 *            the network address.
 * @param scope_id
 *            the scope id for link- or site-local addresses.
 * @return the Inet6Address instance representing the IP address.
 * @throws UnknownHostException
 *             if the address is null or has an invalid length.
 */
ECode CInet6Address::GetByAddress(
    /* [in] */ const String& host,
    /* [in] */ ArrayOf<Byte>* addr,
    /* [in] */ Int32 scopeId,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);
    if (addr == NULL || addr->GetLength() != 16) {
//        throw new UnknownHostException("Illegal IPv6 address");
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    if (scopeId < 0) {
        scopeId = 0;
    }
    return CInet6Address::New(addr, host, scopeId, address);
}

/**
 * Gets an IPv6 address instance according to the given {@code host},
 * {@code addr} and {@code nif}. {@code scope_id} is set according to the
 * given {@code nif} and the {@code addr} type (for example site-local or
 * link-local).
 *
 * @param host
 *            the hostname associated with the address.
 * @param addr
 *            the network address.
 * @param nif
 *            the network interface that this address is associated with.
 * @return the Inet6Address instance representing the IP address.
 * @throws UnknownHostException
 *             if the address is {@code null} or has an invalid length or
 *             the interface doesn't have a numeric scope id for the given
 *             address type.
 */
ECode CInet6Address::GetByAddress(
    /* [in] */ const String& host,
    /* [in] */ ArrayOf<Byte>* addr,
    /* [in] */ INetworkInterface* nif,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);

    FAIL_RETURN(GetByAddress(host, addr, 0, address));

    // if nif is null, nothing needs to be set.
    if (NULL == nif) {
        return NOERROR;
    }

    AutoPtr<IEnumeration> em;
    nif->GetInetAddresses((IEnumeration**)&em);
    Boolean hasNext;
    while(em->HasMoreElements(&hasNext), hasNext)
    {
        AutoPtr<IInetAddress> ia;
        em->NextElement((IInterface**)&ia);
        AutoPtr<ArrayOf<Byte> > addresses;
        ia->GetAddress((ArrayOf<Byte>**)&addresses);
        if(addresses->GetLength() == 16)
        {
            AutoPtr<IInet6Address> v6ia = (IInet6Address*)ia->Probe(EIID_IInet6Address);
            if(v6ia != NULL){
                CInet6Address* cv6ia = (CInet6Address*)(v6ia.Get());
                Boolean isSameType = cv6ia->CompareLocalType((IInet6Address*)*address);
                if(isSameType)
                {
                    ((CInet6Address*)(*address))->mScopeIdSet = TRUE;
                    ((CInet6Address*)(*address))->mScopeId = cv6ia->mScopeId;
                    ((CInet6Address*)(*address))->mScopeIfnameSet = TRUE;
                    nif->GetName(&(((CInet6Address*)(*address))->mIfname));
                    break;
                }
            }
        }
    }
    if(!((CInet6Address*)(*address))->mScopeIdSet)
    {
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    return NOERROR;
}

ECode CInet6Address::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return InetAddress::Equals(obj, result);
}

ECode CInet6Address::ToString(
    /* [out] */ String* result)
{
    FAIL_RETURN(InetAddress::ToString(result));
    if(mIfname.IsNull())
    {
        *result = *result + String("%") + mIfname;
    }else{
        *result = *result + String("%") + mScopeId;
    }
    return NOERROR;
}

Boolean CInet6Address::CompareLocalType(
    /* [in] */ IInet6Address* ia)
{
    Boolean iaSLA, isSLA;
    ia->IsSiteLocalAddress(&iaSLA);
    IsSiteLocalAddress(&isSLA);
    if (iaSLA && isSLA) {
        return TRUE;
    }
    Boolean iaLLA, isLLA;
    ia->IsLinkLocalAddress(&iaLLA);
    IsLinkLocalAddress(&isLLA);
    if (iaLLA && isLLA) {
        return TRUE;
    }
    if (!iaSLA && !iaLLA) {
        return TRUE;
    }
    return FALSE;
}

ECode CInet6Address::GetAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    VALIDATE_NOT_NULL(address);
    return InetAddress::GetAddress(address);
}

ECode CInet6Address::GetHostAddress(
    /* [out] */ String* hostAddress)
{
    VALIDATE_NOT_NULL(hostAddress);
    return InetAddress::GetHostAddress(hostAddress);
}

ECode CInet6Address::GetHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetHostName(hostName);
}

ECode CInet6Address::GetCanonicalHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetCanonicalHostName(hostName);
}

ECode CInet6Address::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return InetAddress::GetHashCode(hashCode);
}

ECode CInet6Address::IsMulticastAddress(
    /* [out] */ Boolean* isMulticastAddress)
{
    VALIDATE_NOT_NULL(isMulticastAddress);
    // Multicast addresses are prefixed with 11111111 (255)
    *isMulticastAddress = (((*mIpAddress)[0] & 0xff) == 0xff);
    return NOERROR;
}

ECode CInet6Address::IsLoopbackAddress(
    /* [out] */ Boolean* isLoopbackAddress)
{
    VALIDATE_NOT_NULL(isLoopbackAddress);
    // The last word must be 1
    AutoPtr<ArrayOf<Byte> > loopBackAddress;
    LOOPBACK->GetAddress((ArrayOf<Byte>**)&loopBackAddress);
    *isLoopbackAddress = mIpAddress->Equals(loopBackAddress);
    return NOERROR;
}

ECode CInet6Address::IsLinkLocalAddress(
    /* [out] */ Boolean* isLinkLocalAddress)
{
    VALIDATE_NOT_NULL(isLinkLocalAddress);
    // the first 10 bits need to be 1111111010 (1018)
    *isLinkLocalAddress = (((*mIpAddress)[0] & 0xff) == 0xfe) &&
            (((*mIpAddress)[1] & 0xc0) == 0x80);
    return NOERROR;
}

ECode CInet6Address::IsSiteLocalAddress(
    /* [out] */ Boolean* isSiteLocalAddress)
{
    VALIDATE_NOT_NULL(isSiteLocalAddress);
    // the first 10 bits need to be 1111111011 (1019)
    *isSiteLocalAddress = (((*mIpAddress)[0] & 0xff) == 0xfe) &&
            (((*mIpAddress)[1] & 0xc0) == 0xc0);
    return NOERROR;
}

ECode CInet6Address::IsMCGlobal(
    /* [out] */ Boolean* isMCGlobal)
{
    VALIDATE_NOT_NULL(isMCGlobal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0xE
    *isMCGlobal = (((*mIpAddress)[0] & 0xff) == 0xff) &&
            (((*mIpAddress)[1] & 0x0f) == 0x0e);
    return NOERROR;
}

ECode CInet6Address::IsMCNodeLocal(
    /* [out] */ Boolean* isMCNodeLocal)
{
    VALIDATE_NOT_NULL(isMCNodeLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x1
    *isMCNodeLocal = (((*mIpAddress)[0] & 0xff) == 0xff) &&
            (((*mIpAddress)[1] & 0x0f) == 0x01);
    return NOERROR;
}

ECode CInet6Address::IsMCLinkLocal(
    /* [out] */ Boolean* isMCLinkLocal)
{
    VALIDATE_NOT_NULL(isMCLinkLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x2
    *isMCLinkLocal = (((*mIpAddress)[0] & 0xff) == 0xff) &&
            (((*mIpAddress)[1] & 0x0f) == 0x02);
    return NOERROR;
}

ECode CInet6Address::IsMCSiteLocal(
    /* [out] */ Boolean* isMCSiteLocal)
{
    VALIDATE_NOT_NULL(isMCSiteLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x5
    *isMCSiteLocal = (((*mIpAddress)[0] & 0xff) == 0xff) &&
            (((*mIpAddress)[1] & 0x0f) == 0x05);
    return NOERROR;
}

ECode CInet6Address::IsMCOrgLocal(
    /* [out] */ Boolean* isMCOrgLocal)
{
    VALIDATE_NOT_NULL(isMCOrgLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x8
    *isMCOrgLocal = (((*mIpAddress)[0] & 0xff) == 0xff) &&
            (((*mIpAddress)[1] & 0x0f) == 0x08);
    return NOERROR;
}

ECode CInet6Address::IsAnyLocalAddress(
    /* [out] */ Boolean* isAnyLocalAddress)
{
    VALIDATE_NOT_NULL(isAnyLocalAddress);
    AutoPtr<ArrayOf<Byte> > anyIpaddress;
    ANY->GetAddress((ArrayOf<Byte>**)&anyIpaddress);
    *isAnyLocalAddress = mIpAddress->Equals(anyIpaddress);
    return NOERROR;
}

ECode CInet6Address::IsReachable(
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachable(timeout, isReachable);
}

ECode CInet6Address::IsReachableEx(
    /* [in] */ INetworkInterface* networkInterface,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachableEx(networkInterface, ttl, timeout, isReachable);
}

ECode CInet6Address::GetScopeId(
    /* [out] */ Int32* scopeId)
{
    VALIDATE_NOT_NULL(scopeId);
    if (mScopeIdSet) {
        *scopeId = mScopeId;
        return NOERROR;
    }
    *scopeId = 0;
    return NOERROR;
}

ECode CInet6Address::GetScopedInterface(
    /* [out] */ INetworkInterface** scopedInterface)
{
    VALIDATE_NOT_NULL(scopedInterface);
    if (mScopeIfnameSet && (mIfname.IsNull())) {
        NetworkInterface::GetByName(mIfname, scopedInterface);
    }
    else{
        scopedInterface = NULL;
    }
    return NOERROR;
}

ECode CInet6Address::IsIPv4CompatibleAddress(
    /* [out] */ Boolean* isIPv4Compatible)
{
    VALIDATE_NOT_NULL(isIPv4Compatible);
    for (Int32 i = 0; i < 12; i++) {
        if ((*mIpAddress)[i] != 0) {
            *isIPv4Compatible = FALSE;
            return NOERROR;
        }
    }
    *isIPv4Compatible = TRUE;
    return NOERROR;
}


// private void writeObject(ObjectOutputStream stream) throws IOException {
//     ObjectOutputStream.PutField fields = stream.putFields();
//     if (ipaddress == null) {
//         fields.put("ipaddress", null);
//     } else {
//         fields.put("ipaddress", ipaddress);
//     }

//     fields.put("scope_id", scope_id);
//     fields.put("scope_id_set", scope_id_set);
//     fields.put("scope_ifname_set", scope_ifname_set);
//     fields.put("ifname", ifname);
//     stream.writeFields();
// }

// private void readObject(ObjectInputStream stream) throws IOException,
//         ClassNotFoundException {
//     ObjectInputStream.GetField fields = stream.readFields();
//     ipaddress = (byte[]) fields.get("ipaddress", null);
//     scope_id = fields.get("scope_id", 0);
//     scope_id_set = fields.get("scope_id_set", false);
//     ifname = (String) fields.get("ifname", null);
//     scope_ifname_set = fields.get("scope_ifname_set", false);
//     if (scope_ifname_set && null != ifname) {
//         scopedIf = NetworkInterface.getByName(ifname);
//     }
// }

// /**
//  * Returns a string containing a concise, human-readable description of this
//  * IP address.
//  *
//  * @return the description, as host/address.
//  */
// @Override
// public String toString() {
//     if (ifname != null) {
//         return super.toString() + "%" + ifname;
//     }
//     if (scope_id != 0) {
//         return super.toString() + "%" + scope_id;
//     }
//     return super.toString();
// }

ECode CInet6Address::constructor(
    /* [in] */ ArrayOf<Byte>* address,
    /* [in] */ const String& name,
    /* [in] */ Int32 scopeId)
{
    mFamily = AF_INET6;
    mHostName = name;
    mIpAddress = address;
    mScopeId = scopeId;
    if (scopeId != 0) {
        mScopeIdSet = TRUE;
    }
    return NOERROR;
}

ECode CInet6Address::GetANY(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ANY;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CInet6Address::GetLOOPBACK(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = LOOPBACK;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
