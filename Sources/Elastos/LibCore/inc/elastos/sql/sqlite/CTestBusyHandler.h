
#ifndef __CTESTBUSYHANDLER_H__
#define __CTESTBUSYHANDLER_H__

#include "_CTestBusyHandler.h"
#include <cmdef.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CTestBusyHandler)
{
public:

    CARAPI Busy(
        /* [in] */ const String& table,
        /* [in] */ Int32 count,
        /* [out] */ Boolean* value);

    CARAPI Columns(
        /* [in] */ const ArrayOf<String>& coldata);

    CARAPI Types(
        /* [in] */ const ArrayOf<String>& types);

    CARAPI Newrow(
        /* [in] */ const ArrayOf<String>& rowdata,
        /* [out] */ Boolean* value);

    CARAPI constructor();
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CTESTBUSYHANDLER_H__
