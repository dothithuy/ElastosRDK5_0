
#include "CAuthorizerCallback.h"

using Elastos::Sql::SQLite::EIID_IAuthorizer;
using Elastos::Sql::SQLite::EIID_ICallback;

namespace Elastos {
namespace Sql {
namespace SQLite {

PInterface CAuthorizerCallback::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IAuthorizer) {
        return (IAuthorizer*)this;
    } else if (riid == EIID_ICallback) {
        return (ICallback*)this;
    } else {
        return NULL;
    }
}

ECode CAuthorizerCallback::Authorize(
    /* [in] */ Int32 what,
    /* [in] */ const String& arg1,
    /* [in] */ const String& arg2,
    /* [in] */ const String& arg3,
    /* [in] */ const String& arg4,
    /* [out] */ Int32* code)
{
    printf("DB authorization callback  what:%d  arg1:%s  arg2:%s  arg3:%s  arg4:%s \n",
            what, arg1.string() ,arg2.string(), arg3.string(), arg4.string());
    isAuthorizing = TRUE;
    if (what != IConstants::SQLITE_SELECT || arg1.Contains("private_table")) {
        *code = IConstants::SQLITE_DENY;
    } else {
        *code = IConstants::SQLITE_OK;
    }
    return NOERROR;
}

ECode CAuthorizerCallback::Columns(
    /* [in] */ const ArrayOf<String>& coldata)
{
    return NOERROR;
}

ECode CAuthorizerCallback::Types(
    /* [in] */ const ArrayOf<String>& types)
{
    return NOERROR;
}

ECode CAuthorizerCallback::Newrow(
    /* [in] */ const ArrayOf<String>& rowdata,
    /* [out] */ Boolean* value)
{
    *value = FALSE;
    return NOERROR;
}

ECode CAuthorizerCallback::constructor()
{
    isAuthorizing = FALSE;
    return NOERROR;
}

Boolean CAuthorizerCallback::WasCalled()
{
    return isAuthorizing;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos