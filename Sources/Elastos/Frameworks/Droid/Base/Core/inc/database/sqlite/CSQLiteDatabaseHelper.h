
#ifndef __CSQLITEDATABASEHELPER_H__
#define __CSQLITEDATABASEHELPER_H__

#include "_CSQLiteDatabaseHelper.h"

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

CarClass(CSQLiteDatabaseHelper)
{
public:
    CARAPI ReleaseMemory(
        /* [out] */ Int32* result);

    CARAPI OpenDatabase(
        /* [in] */ const String& path,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [in] */ Int32 flags,
        /* [out] */ ISQLiteDatabase** result);

    CARAPI OpenDatabaseEx(
        /* [in] */ const String& path,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [in] */ Int32 flags,
        /* [in] */ IDatabaseErrorHandler* errorHandler,
        /* [out] */ ISQLiteDatabase** result);

    CARAPI OpenOrCreateDatabase(
        /* [in] */ IFile* file,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [out] */ ISQLiteDatabase** result);

    CARAPI OpenOrCreateDatabaseEx(
        /* [in] */ const String& path,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [out] */ ISQLiteDatabase** result);

    CARAPI OpenOrCreateDatabaseEx2(
        /* [in] */ const String& path,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [in] */ IDatabaseErrorHandler* errorHandler,
        /* [out] */ ISQLiteDatabase** result);

    CARAPI DeleteDatabase(
        /* [in] */ IFile* file,
        /* [out] */ Boolean* result);

    CARAPI Create(
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [out] */ ISQLiteDatabase** result);

    CARAPI FindEditTable(
        /* [in] */ const String& tables,
        /* [out] */ String* str);
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif //__CSQLITEDATABASEHELPER_H__