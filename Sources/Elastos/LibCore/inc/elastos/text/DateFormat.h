
#ifndef __DATEFORMAT_H__
#define __DATEFORMAT_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "Elastos.CoreLibrary_server.h"
#include "Format.h"
#include <elastos/HashMap.h>
#include "AttributedCharacterIteratorAttributeMacro.h"

using Elastos::Utility::IDate;
using Elastos::Utility::ICalendar;
using Elastos::Utility::ITimeZone;
using Libcore::ICU::ILocale;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;
using Elastos::Utility::HashMap;

namespace Elastos {
namespace Text {

class DateFormat : public Format
{
public:
    class Field
        : public ElRefBase
        , public IDateFormatField
        , public Format::Field
    {
    public:
        CAR_INTERFACE_DECL()
        TEXTATTRIBUITEDCHARACTERITERATORATTRIBUTE_METHODS_DECL()

        Field();

        CARAPI Init(
            /* [in] */ const String& fieldName,
            /* [in] */ Int32 calendarField);

        // CARAPI GetName(
        //     /* [out] */ String* name);

        CARAPI GetCalendarField(
            /* [out] */ Int32* field);

        static CARAPI OfCalendarField(
            /* [in] */ Int32 calendarField,
            /* [out] */ IDateFormatField** field);

    public:

        static const AutoPtr<IDateFormatField> ERA;

        static const AutoPtr<IDateFormatField> YEAR;

        static const AutoPtr<IDateFormatField> MONTH;

        static const AutoPtr<IDateFormatField> HOUR_OF_DAY0;

        static const AutoPtr<IDateFormatField> HOUR_OF_DAY1;

        static const AutoPtr<IDateFormatField> MINUTE;

        static const AutoPtr<IDateFormatField> SECOND;

        static const AutoPtr<IDateFormatField> MILLISECOND;

        static const AutoPtr<IDateFormatField> DAY_OF_WEEK;

        static const AutoPtr<IDateFormatField> DAY_OF_MONTH;

        static const AutoPtr<IDateFormatField> DAY_OF_YEAR;

        static const AutoPtr<IDateFormatField> DAY_OF_WEEK_IN_MONTH;

        static const AutoPtr<IDateFormatField> WEEK_OF_YEAR;

        static const AutoPtr<IDateFormatField> WEEK_OF_MONTH;

        static const AutoPtr<IDateFormatField> AM_PM;

        static const AutoPtr<IDateFormatField> HOUR0;

        static const AutoPtr<IDateFormatField> HOUR1;

        static const AutoPtr<IDateFormatField> TIME_ZONE;

    private:

        static HashMap<Int32, AutoPtr<IDateFormatField> > sTable;

        Int32 mCalendarField;
    };

public:
    //@Override
    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value);

    CARAPI FormatDate(
        /* [in] */ IDate* date,
        /* [out] */ String* result);

    virtual CARAPI FormatDateEx(
        /* [in] */ IDate* date,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** result) = 0;

    static CARAPI GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** locales);

    virtual CARAPI GetCalendar(
        /* [out] */ ICalendar** calendar);

    static CARAPI GetDateInstance(
        /* [out] */ IDateFormat** instance);

    static CARAPI GetDateInstanceEx(
        /* [in] */ Int32 style,
        /* [out] */ IDateFormat** instance);

    static CARAPI GetDateInstanceEx2(
        /* [in] */ Int32 style,
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormat** instance);

    static CARAPI GetDateTimeInstance(
        /* [out] */ IDateFormat** instance);

    static CARAPI GetDateTimeInstanceEx(
        /* [in] */ Int32 dateStyle,
        /* [in] */ Int32 timeStyle,
        /* [out] */ IDateFormat** instance);

    static CARAPI GetDateTimeInstanceEx2(
        /* [in] */ Int32 dateStyle,
        /* [in] */ Int32 timeStyle,
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormat** instance);

    static CARAPI GetInstance(
        /* [out] */ IDateFormat** instance);

    virtual CARAPI GetNumberFormat(
        /* [out] */ INumberFormat** numberFormat);

    static CARAPI GetTimeInstance(
        /* [out] */ IDateFormat** instance);

    static CARAPI GetTimeInstanceEx(
        /* [in] */ Int32 style,
        /* [out] */ IDateFormat** instance);

    static CARAPI GetTimeInstanceEx2(
        /* [in] */ Int32 style,
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormat** instance);

    virtual CARAPI GetTimeZone(
        /* [out] */ ITimeZone** timeZone);

    virtual CARAPI IsLenient(
        /* [out] */ Boolean* isLenient);

    virtual CARAPI Parse(
        /* [in] */ const String& string,
        /* [out] */ IDate** date);

    virtual CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IDate** date) = 0;

    //@Override
    CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    virtual CARAPI SetCalendar(
        /* [in] */ ICalendar* cal);

    virtual CARAPI SetLenient(
        /* [in] */ Boolean value);

    virtual CARAPI SetNumberFormat(
       /* [in] */ INumberFormat* format);

    virtual CARAPI SetTimeZone(
        /* [in] */ ITimeZone* timezone);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

protected:
    DateFormat();

    //@Override
    //public boolean equals(Object object);

    // virtual CARAPI_(PInterface) Probe(
    //     /* [in] */ REIID riid) = 0;

private:
    static CARAPI CheckDateStyle(
        /* [in] */ Int32 style);

    static CARAPI CheckTimeStyle(
        /* [in] */ Int32 style);

protected:
    AutoPtr<ICalendar> mCalendar;

    AutoPtr<INumberFormat> mNumberFormat;
};

} // namespace Text
} // namespace Elastos

#endif //__DATEFORMAT_H__
