
#include "CDatabaseX.h"
#include "CObject.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CAR_OBJECT_IMPL(CDatabaseX)

CDatabaseX::CDatabaseX()
{
    Elastos::Core::CObject::New((ISynchronize**)&mLock);
}

ECode CDatabaseX::Exec(
    /* [in] */ const String& sql,
    /* [in] */ ICallback * cb)
{
    ECode ec = NOERROR;
    ec = Exec(sql, cb);
    mLock->Lock();
    mLock->NotifyAll();
    mLock->Unlock();
    return ec;
}

ECode CDatabaseX::Exec(
    /* [in] */ const String& sql,
    /* [in] */ ICallback * cb,
    /* [in] */ const ArrayOf<String>& args)
{
    ECode ec = NOERROR;
    ec = Exec(sql, cb, args);
    mLock->Lock();
    mLock->NotifyAll();
    mLock->Unlock();
    return ec;
}

ECode CDatabaseX::GetTable(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String>& args,
    /* [out] */ ITableResult ** tbl)
{
    ECode ec = NOERROR;
    ec = GetTable(sql, args, tbl);
    mLock->Lock();
    mLock->NotifyAll();
    mLock->Unlock();
    return ec;
}

ECode CDatabaseX::GetTable(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String>& args,
    /* [in] */ ITableResult * tbl)
{
    ECode ec = NOERROR;
    ec = GetTable(sql, args, tbl);
    mLock->Lock();
    mLock->NotifyAll();
    mLock->Unlock();
    return ec;
}

void CDatabaseX::Wait(Int32 ms)
{
    mLock->Lock();
    mLock->Wait(ms);
    mLock->Unlock();
}

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos
