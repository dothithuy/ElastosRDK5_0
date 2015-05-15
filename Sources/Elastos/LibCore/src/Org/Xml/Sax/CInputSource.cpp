
#include "CInputSource.h"

namespace Org {
namespace Xml {
namespace Sax {

ECode CInputSource::SetPublicId(
    /* [in] */ const String& publicId)
{
    this->mPublicId = publicId;
    return NOERROR;
}

ECode CInputSource::GetPublicId(
    /* [out] */ String* publicId)
{
    VALIDATE_NOT_NULL(publicId);

    *publicId = this->mPublicId;

    return NOERROR;
}

ECode CInputSource::SetSystemId(
    /* [in] */ const String& systemId)
{
    this->mSystemId = systemId;

    return NOERROR;
}

ECode CInputSource::GetSystemId(
    /* [out] */ String* systemId)
{
    VALIDATE_NOT_NULL(systemId);

    *systemId = this->mSystemId;

    return NOERROR;
}

ECode CInputSource::SetByteStream(
    /* [in] */ Elastos::IO::IInputStream* byteStream)
{
    this->mByteStream = byteStream;

    return NOERROR;
}

ECode CInputSource::GetByteStream(
    /* [out] */ Elastos::IO::IInputStream** byteStream)
{
    VALIDATE_NOT_NULL(byteStream);

    *byteStream = (this->mByteStream).Get();

    return NOERROR;
}

ECode CInputSource::SetEncoding(
    /* [in] */ const String& encoding)
{
    this->mEncoding = encoding;

    return NOERROR;
}

ECode CInputSource::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);

    *encoding = this->mEncoding;

    return NOERROR;
}

ECode CInputSource::SetCharacterStream(
    /* [in] */ Elastos::IO::IReader* characterStream)
{
    this->mCharacterStream = characterStream;

    return NOERROR;
}

ECode CInputSource::GetCharacterStream(
    /* [out] */ Elastos::IO::IReader** characterStream)
{
    VALIDATE_NOT_NULL(characterStream);

    *characterStream = this->mCharacterStream.Get();

    return NOERROR;
}

ECode CInputSource::constructor()
{
    return NOERROR;
}

ECode CInputSource::constructor(
    /* [in] */ const String& systemId)
{
    return SetSystemId(systemId);
}

ECode CInputSource::constructor(
    /* [in] */ Elastos::IO::IInputStream* byteStream)
{
    return SetByteStream(byteStream);
}

ECode CInputSource::constructor(
    /* [in] */ Elastos::IO::IReader* characterStream)
{
    return SetCharacterStream(characterStream);;
}

} // namespace Sax
} // namespace Xml
} // namespace Org