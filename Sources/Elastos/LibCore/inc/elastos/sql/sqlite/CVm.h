
#ifndef __CVM_H__
#define __CVM_H__

#include "_CVm.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CVm)
{
public:
    CARAPI Step(
        /* [in] */ ICallback * cb,
        /* [out] */ Boolean * value);

    CARAPI Compile(
        /* [out] */ Boolean * value);

    CARAPI Stop();

    CARAPI Finalize();

    CVm();

public:
    Int32 mError_code;
    Int64 mHandle;

private:
    static CARAPI_(Boolean) Internal_init();

private:
    static Boolean isinit;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CVM_H__
