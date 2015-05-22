
#ifndef __X509CERTIFICATE_H__
#define __X509CERTIFICATE_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "Certificate.h"

using Elastos::Utility::IDate;
using Elastos::Utility::IList;
using Elastos::Utility::ICollection;
using Elastos::Math::IBigInteger;
using Elastos::Security::Cert::IX509Extension;
using Elastos::Security::IPrincipal;
using Elastos::Security::Cert::Certificate;
using Elastosx::Security::Auth::X500::IX500Principal;

namespace Elastos {
namespace Security {
namespace Cert {

class X509Certificate
    : public ElRefBase
    , public Certificate
    , public IX509Extension {
public:
    CAR_INTERFACE_DECL()
    /**
     * Checks whether the certificate is currently valid.
     * <p>
     * The validity defined in ASN.1:
     *
     * <pre>
     * validity             Validity
     *
     * Validity ::= SEQUENCE {
     *      notBefore       CertificateValidityDate,
     *      notAfter        CertificateValidityDate }
     *
     * CertificateValidityDate ::= CHOICE {
     *      utcTime         UTCTime,
     *      generalTime     GeneralizedTime }
     * </pre>
     *
     * @throws CertificateExpiredException
     *             if the certificate has expired.
     * @throws CertificateNotYetValidException
     *             if the certificate is not yet valid.
     */
    virtual CARAPI CheckValidity() = 0;

    /**
     * Checks whether the certificate is valid at the specified date.
     *
     * @param date
     *            the date to check the validity against.
     * @throws CertificateExpiredException
     *             if the certificate has expired.
     * @throws CertificateNotYetValidException
     *             if the certificate is not yet valid.
     * @see #checkValidity()
     */
    virtual CARAPI CheckValidity(
        /* [out] */ IDate** date) = 0;

    /**
     * Returns the certificates {@code version} (version number).
     * <p>
     * The version defined is ASN.1:
     *
     * <pre>
     * Version ::=  INTEGER  {  v1(0), v2(1), v3(2)  }
     * </pre>
     *
     * @return the version number.
     */
     virtual CARAPI GetVersion(
        /* [out] */ Int32* version) = 0 ;

    /**
     * Returns the {@code serialNumber} of the certificate.
     * <p>
     * The ASN.1 definition of {@code serialNumber}:
     *
     * <pre>
     * CertificateSerialNumber  ::=  INTEGER
     * </pre>
     *
     * @return the serial number.
     */
    virtual CARAPI GetSerialNumber(
        /* [out] */ IBigInteger** number) = 0;

    /**
     * Returns the {@code issuer} (issuer distinguished name) as an
     * implementation specific {@code Principal} object.
     * <p>
     * The ASN.1 definition of {@code issuer}:
     *
     * <pre>
     *  issuer      Name
     *
     *  Name ::= CHOICE {
     *      RDNSequence }
     *
     *    RDNSequence ::= SEQUENCE OF RelativeDistinguishedName
     *
     *    RelativeDistinguishedName ::= SET OF AttributeTypeAndValue
     *
     *    AttributeTypeAndValue ::= SEQUENCE {
     *      type     AttributeType,
     *      value    AttributeValue }
     *
     *    AttributeType ::= OBJECT IDENTIFIER
     *
     *    AttributeValue ::= ANY DEFINED BY AttributeType
     * </pre>
     *
     * <b>replaced by:</b> {@link #getIssuerX500Principal()}.
     *
     * @return the {@code issuer} as an implementation specific {@code
     *         Principal}.
     */
    virtual CARAPI GetIssuerDN(
        /* [out] */ IPrincipal** issuerDN) = 0;

    /**
     * Returns the {@code issuer} (issuer distinguished name) as an {@code
     * X500Principal}.
     *
     * @return the {@code issuer} (issuer distinguished name).
     */
    virtual CARAPI GetIssuerX500Principal(
        /* [out] */ IX500Principal** issuer);

    /**
     * Returns the {@code subject} (subject distinguished name) as an
     * implementation specific {@code Principal} object.
     * <p>
     * The ASN.1 definition of {@code subject}:
     *
     * <pre>
     * subject      Name
     *
     *  Name ::= CHOICE {
     *      RDNSequence }
     *
     *    RDNSequence ::= SEQUENCE OF RelativeDistinguishedName
     *
     *    RelativeDistinguishedName ::= SET OF AttributeTypeAndValue
     *
     *    AttributeTypeAndValue ::= SEQUENCE {
     *      type     AttributeType,
     *      value    AttributeValue }
     *
     *    AttributeType ::= OBJECT IDENTIFIER
     *
     *    AttributeValue ::= ANY DEFINED BY AttributeType
     * </pre>
     *
     * <p>
     * <b>replaced by:</b> {@link #getSubjectX500Principal()}.
     *
     * @return the {@code subject} (subject distinguished name).
     */
    virtual CARAPI GetSubjectDN(
        /* [out] */ IPrincipal** subDN) = 0;

    /**
     * Returns the {@code subject} (subject distinguished name) as an {@code
     * X500Principal}.
     *
     * @return the {@code subject} (subject distinguished name)
     */
    virtual CARAPI GetSubjectX500Principal(
        /* [out] */ IX500Principal** principal);

    /**
     * Returns the {@code notBefore} date from the validity period of the
     * certificate.
     *
     * @return the start of the validity period.
     */
    virtual CARAPI GetNotBefore(
        /* [out] */ IDate** date) = 0;


    /**
     * Returns the {@code notAfter} date of the validity period of the
     * certificate.
     *
     * @return the end of the validity period.
     */
    virtual CARAPI GetNotAfter(
        /* [out] */ IDate** date) = 0;

    /**
     * Returns the {@code tbsCertificate} information from this certificate in
     * DER-encoded format.
     *
     * @return the DER-encoded certificate information.
     * @throws CertificateEncodingException
     *             if an error occurs in encoding
     */
    virtual CARAPI GetTBSCertificate(
        /* [out, callee] */ ArrayOf<Byte>** cert) = 0;

    /**
     * Returns the raw signature bits from the certificate.
     *
     * @return the raw signature bits from the certificate.
     */
    virtual CARAPI GetSignature(
        /* [out, callee] */ ArrayOf<Byte>** sign) = 0;

    /**
     * Returns the name of the algorithm for the certificate signature.
     *
     * @return the signature algorithm name.
     */
    virtual CARAPI GetSigAlgName(
        /* [out] */ String* name) = 0;

    /**
     * Returns the OID of the signature algorithm from the certificate.
     *
     * @return the OID of the signature algorithm.
     */
    virtual CARAPI GetSigAlgOID(
        /* [out] */ String* oid) = 0;

    /**
     * Returns the parameters of the signature algorithm in DER-encoded format.
     *
     * @return the parameters of the signature algorithm, or {@code null} if
     *         none are used.
     */
    virtual CARAPI GetSigAlgParams(
        /* [out, callee] */ ArrayOf<Byte>** param) = 0;

    /**
     * Returns the {@code issuerUniqueID} from the certificate.
     *
     * @return the {@code issuerUniqueID} or {@code null} if there's none in the
     *         certificate.
     */
    virtual CARAPI GetIssuerUniqueID(
        /* [out, callee] */ ArrayOf<Boolean>** id) = 0;

    /**
     * Returns the {@code subjectUniqueID} from the certificate.
     *
     * @return the {@code subjectUniqueID} or null if there's none in the
     *         certificate.
     */
    virtual CARAPI GetSubjectUniqueID(
        /* [out, callee] */ ArrayOf<Boolean>** id) = 0;

    /**
     * Returns the {@code KeyUsage} extension as a {@code boolean} array.
     * <p>
     * The ASN.1 definition of {@code KeyUsage}:
     *
     * <pre>
     * KeyUsage ::= BIT STRING {
     *      digitalSignature        (0),
     *      nonRepudiation          (1),
     *      keyEncipherment         (2),
     *      dataEncipherment        (3),
     *      keyAgreement            (4),
     *      keyCertSign             (5),
     *      cRLSign                 (6),
     *      encipherOnly            (7),
     *      decipherOnly            (8) }
     *
     * </pre>
     *
     * @return the {@code KeyUsage} extension or {@code null} if there's none in
     *         the certificate.
     */
    virtual CARAPI GetKeyUsage(
        /* [out, callee] */ ArrayOf<Boolean>** usage) = 0;

    /**
     * Returns a read-only list of OID strings representing the {@code
     * ExtKeyUsageSyntax} field of the extended key usage extension.
     *
     * @return the extended key usage extension, or {@code null} if there's none
     *         in the certificate.
     * @throws CertificateParsingException
     *             if the extension decoding fails.
     */
    virtual CARAPI GetExtendedKeyUsage(
        /* [out] */ IList** usage);

    /**
     * Returns the path length of the certificate constraints from the {@code
     * BasicContraints} extension.
     *
     * @return the path length of the certificate constraints if the extension
     *         is present or {@code -1} if the extension is not present. {@code
     *         Integer.MAX_VALUE} if there's not limit.
     */
    virtual CARAPI GetBasicConstraints(
        /* [out] */ Int32* constraints) = 0;

    /**
     * Returns a read-only list of the subject alternative names from the
     * {@code SubjectAltName} extension.
     * <p>
     * The ASN.1 definition of {@code SubjectAltName}:
     *
     * <pre>
     * SubjectAltName ::= GeneralNames
     *
     * GeneralNames ::= SEQUENCE SIZE (1..MAX) OF GeneralName
     *
     * GeneralName ::= CHOICE {
     *      otherName                       [0]     AnotherName,
     *      rfc822Name                      [1]     IA5String,
     *      dNSName                         [2]     IA5String,
     *      x400Address                     [3]     ORAddress,
     *      directoryName                   [4]     Name,
     *      ediPartyName                    [5]     EDIPartyName,
     *      uniformResourceIdentifier       [6]     IA5String,
     *      iPAddress                       [7]     OCTET STRING,
     *      registeredID                    [8]     OBJECT IDENTIFIER }
     *
     * </pre>
     *
     * @return the subject alternative names or {@code null} if there are none
     *         in the certificate.
     * @throws CertificateParsingException
     *             if decoding of the extension fails.
     */
    virtual CARAPI GetSubjectAlternativeNames(
        /* [out] */ ICollection** names);

    /**
     * Returns a read-only list of the issuer alternative names from the {@code
     * IssuerAltName} extension.
     * <p>
     * The ASN.1 definition of {@code IssuerAltName}:
     *
     * <pre>
     * IssuerAltName ::= GeneralNames
     *
     * GeneralNames ::= SEQUENCE SIZE (1..MAX) OF GeneralName
     *
     * GeneralName ::= CHOICE {
     *      otherName                       [0]     AnotherName,
     *      rfc822Name                      [1]     IA5String,
     *      dNSName                         [2]     IA5String,
     *      x400Address                     [3]     ORAddress,
     *      directoryName                   [4]     Name,
     *      ediPartyName                    [5]     EDIPartyName,
     *      uniformResourceIdentifier       [6]     IA5String,
     *      iPAddress                       [7]     OCTET STRING,
     *      registeredID                    [8]     OBJECT IDENTIFIER }
     *
     * </pre>
     *
     * @return the issuer alternative names of {@code null} if there are none in
     *         the certificate.
     * @throws CertificateParsingException
     *             if decoding of the extension fails.
     */
    virtual CARAPI GetIssuerAlternativeNames(
        /* [out] */ ICollection** names);
protected:
    /**
     * Creates a new {@code X509Certificate}.
     */
    X509Certificate();
private:

};

} // end Cert
} // end Security
} // end Elastos

#endif // __X509CERTIFICATE_H__