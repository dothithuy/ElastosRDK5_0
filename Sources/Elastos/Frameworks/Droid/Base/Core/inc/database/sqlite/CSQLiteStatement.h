#ifndef __CSQLITESTATEMENT_H__
#define __CSQLITESTATEMENT_H__

#include "_CSQLiteStatement.h"
#include "database/sqlite/SQLiteProgram.h"

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

CarClass(CSQLiteStatement), SQLiteProgram
{
public:
    CARAPI Execute();

    CARAPI ExecuteUpdateDelete(
        /* [out] */ Int32* result);

    CARAPI ExecuteInsert(
        /* [out] */ Int64* value);

    CARAPI SimpleQueryForInt64(
        /* [out] */ Int64* value);

    CARAPI SimpleQueryForString(
        /* [out] */ String* value);

    CARAPI SimpleQueryForBlobFileDescriptor(
        /* [out] */ IParcelFileDescriptor** result);

    CARAPI GetUniqueId(
        /* [out] */ Int32 *value);

    CARAPI BindNull(
        /* [in] */ Int32 index);

    CARAPI BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    CARAPI BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value);

    CARAPI ClearBindings();

    CARAPI BindAllArgsAsStrings(
        /* [in] */ ArrayOf<String>* bindArgs);

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI Close();

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI constructor(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& sql,
        /* [in] */ ArrayOf<IInterface*>* bindArgs);
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif // __CSQLITESTATEMENT_H__
