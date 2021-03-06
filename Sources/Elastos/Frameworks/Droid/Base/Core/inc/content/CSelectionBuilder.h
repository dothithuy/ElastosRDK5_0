
#ifndef __CSELECTIONBUILDER_H__
#define __CSELECTIONBUILDER_H__

#include "_CSelectionBuilder.h"
#include <ext/frameworkext.h>
#include <elastos/List.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::List;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

CarClass(CSelectionBuilder)
{
public:
    CSelectionBuilder();

    ~CSelectionBuilder();

    /**
     * Reset any internal state, allowing this builder to be recycled.
     */
    CARAPI Reset();

    /**
     * Append the given selection clause to the internal state. Each clause is
     * surrounded with parenthesis and combined using {@code AND}.
     */
    CARAPI Append(
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<IInterface*>* selectionArgs);

    /**
     * Return selection string for current internal state.
     *
     * @see #getSelectionArgs()
     */
    CARAPI GetSelection(
        /* [out] */ String* str);

    /**
     * Return selection arguments for current internal state.
     *
     * @see #getSelection()
     */
    CARAPI GetSelectionArgs(
        /* [out, callee] */ ArrayOf<String>** selectionArgs);

    /**
     * Execute query using the current internal state as {@code WHERE} clause.
     * Missing arguments as treated as {@code null}.
     */
    CARAPI Query(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& orderBy,
        /* [out] */ ICursor** cursor);

    /**
     * Execute query using the current internal state as {@code WHERE} clause.
     */
    CARAPI QueryEx(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ ICursor** cursor);

    /**
     * Execute update using the current internal state as {@code WHERE} clause.
     */
    CARAPI Update(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& table,
        /* [in] */ IContentValues* values,
        /* [out] */ Int32* affectedRow);

    /**
     * Execute delete using the current internal state as {@code WHERE} clause.
     */
    CARAPI Delete(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& table,
        /* [out] */ Int32* affectedRow);

private:
    AutoPtr<StringBuilder> mSelection;
    List<String> mSelectionArgs;

};

}
}
}
}

#endif // __CSELECTIONBUILDER_H__
