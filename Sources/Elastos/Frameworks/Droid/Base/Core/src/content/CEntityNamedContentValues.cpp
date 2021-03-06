
#include "content/CEntityNamedContentValues.h"

namespace Elastos {
namespace Droid {
namespace Content {

CEntityNamedContentValues::CEntityNamedContentValues()
    : mUri(NULL)
    , mContentValues(NULL)
{}

CEntityNamedContentValues::~CEntityNamedContentValues()
{}

ECode CEntityNamedContentValues::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = mUri;
    INTERFACE_ADDREF(*uri);
    return NOERROR;
}

ECode CEntityNamedContentValues::GetValues(
    /* [out] */ IContentValues** values)
{
    VALIDATE_NOT_NULL(values)
    *values = mContentValues;
    INTERFACE_ADDREF(*values);
    return NOERROR;
}

ECode CEntityNamedContentValues::constructor(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values)
{
    mUri = uri;
    mContentValues = values;
    return NOERROR;
}

}
}
}

