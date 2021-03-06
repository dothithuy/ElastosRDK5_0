#include "text/format/DateUtils.h"
#include "text/format/DateFormat.h"
#include "text/format/CTime.h"
#include "content/res/CResourcesHelper.h"
#include "content/res/CResources.h"
#include "os/SystemClock.h"
#include "R.h"
#include <elastos/Math.h>

using Libcore::ICU::ILocale;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocale;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::CLocaleDataHelper;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Text::IDateFormat;
using Elastos::Text::IDateFormatHelper;
using Elastos::Text::CDateFormatHelper;
//using Elastos::Utility::IFormatter;
//using Elastos::Utility::CFormatter;   //java.util.Formatter
using Elastos::Utility::CGregorianCalendar;
using Elastos::Utility::IGregorianCalendar;
using Elastos::Utility::IDate;
using Elastos::Utility::CCalendarHelper;
using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::ITimeZone;

using Elastos::Droid::R;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Content::Res::IResourcesHelper;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

// This table is used to lookup the resource string id of a format string
// used for formatting a start and end date that fall in the same year.
// The index is constructed from a bit-wise OR of the boolean values:
// {showTime, showYear, showWeekDay}.  For example, if showYear and
// showWeekDay are both true, then the index would be 3.
/** @deprecated do not use. */
const Int32 DateUtils::sSameYearTable[] = {
        R::string::same_year_md1_md2,
        R::string::same_year_wday1_md1_wday2_md2,
        R::string::same_year_mdy1_mdy2,
        R::string::same_year_wday1_mdy1_wday2_mdy2,
        R::string::same_year_md1_time1_md2_time2,
        R::string::same_year_wday1_md1_time1_wday2_md2_time2,
        R::string::same_year_mdy1_time1_mdy2_time2,
        R::string::same_year_wday1_mdy1_time1_wday2_mdy2_time2,
        // Numeric date strings
        R::string::numeric_md1_md2,
        R::string::numeric_wday1_md1_wday2_md2,
        R::string::numeric_mdy1_mdy2,
        R::string::numeric_wday1_mdy1_wday2_mdy2,
        R::string::numeric_md1_time1_md2_time2,
        R::string::numeric_wday1_md1_time1_wday2_md2_time2,
        R::string::numeric_mdy1_time1_mdy2_time2,
        R::string::numeric_wday1_mdy1_time1_wday2_mdy2_time2,
    };

// This table is used to lookup the resource string id of a format string
// used for formatting a start and end date that fall in the same month.
// The index is constructed from a bit-wise OR of the boolean values:
// {showTime, showYear, showWeekDay}.  For example, if showYear and
// showWeekDay are both true, then the index would be 3.
/** @deprecated do not use. */
const Int32 DateUtils::sSameMonthTable[] = {
        R::string::same_month_md1_md2,
        R::string::same_month_wday1_md1_wday2_md2,
        R::string::same_month_mdy1_mdy2,
        R::string::same_month_wday1_mdy1_wday2_mdy2,
        R::string::same_month_md1_time1_md2_time2,
        R::string::same_month_wday1_md1_time1_wday2_md2_time2,
        R::string::same_month_mdy1_time1_mdy2_time2,
        R::string::same_month_wday1_mdy1_time1_wday2_mdy2_time2,

        R::string::numeric_md1_md2,
        R::string::numeric_wday1_md1_wday2_md2,
        R::string::numeric_mdy1_mdy2,
        R::string::numeric_wday1_mdy1_wday2_mdy2,
        R::string::numeric_md1_time1_md2_time2,
        R::string::numeric_wday1_md1_time1_wday2_md2_time2,
        R::string::numeric_mdy1_time1_mdy2_time2,
        R::string::numeric_wday1_mdy1_time1_wday2_mdy2_time2,
    };

Mutex DateUtils::sLock;
AutoPtr<IConfiguration> DateUtils::sLastConfig = NULL;
AutoPtr<Elastos::Text::IDateFormat> DateUtils::sStatusTimeFormat = NULL;
String DateUtils::sElapsedFormatMMSS = String(NULL);
String DateUtils::sElapsedFormatHMMSS = String(NULL);

const String DateUtils::FAST_FORMAT_HMMSS("%1$d:%2$02d:%3$02d");
const String DateUtils::FAST_FORMAT_MMSS("%1$02d:%2$02d");
const Char32 DateUtils::TIME_SEPARATOR = ':';

AutoPtr<ITime> DateUtils::sNowTime = NULL;
AutoPtr<ITime> DateUtils::sThenTime = NULL;

Mutex DateUtils::sLockDateUtilsClass;


String DateUtils::GetDayOfWeekString(
/* [in] */ Int32 dayOfWeek,
/* [in] */ Int32 abbrev)
{
    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> defaultLocale;
    localeHelp->GetDefault((ILocale**)&defaultLocale);

    AutoPtr<ILocaleDataHelper> ldh;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
    AutoPtr<ILocaleData> d;
    ldh->Get(defaultLocale, (ILocaleData**)&d);

    AutoPtr< ArrayOf<String> > names;
    switch (abbrev) {
        case IDateUtils::LENGTH_LONG:{
            d->GetLongWeekdayNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_MEDIUM:{
            d->GetShortWeekdayNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORT:{
            d->GetShortWeekdayNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORTER:{
            d->GetShortWeekdayNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORTEST:{
            d->GetTinyWeekdayNames((ArrayOf<String>**)&names);
            break;
        }
        default:{
            d->GetShortWeekdayNames((ArrayOf<String>**)&names);
            break;
        }
    }
    return (*(names))[dayOfWeek];
}

String DateUtils::GetAMPMString(
        /* [in] */ Int32 ampm)
{
    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> defaultLocale;
    localeHelp->GetDefault((ILocale**)&defaultLocale);

    AutoPtr<ILocaleDataHelper> ldh;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
    AutoPtr<ILocaleData> d;
    ldh->Get(defaultLocale, (ILocaleData**)&d);

    AutoPtr< ArrayOf<String> > amPm;
    d->GetAmPm( (ArrayOf<String>**)&amPm );
    return (*amPm)[ampm - ICalendar::AM];
}

String DateUtils::GetMonthString(
    /* [in] */ Int32 month,
    /* [in] */ Int32 abbrev)
{
    // Note that here we use d.shortMonthNames for MEDIUM, SHORT and SHORTER.
    // This is a shortcut to not spam the translators with too many variations
    // of the same string.  If we find that in a language the distinction
    // is necessary, we can can add more without changing this API.
    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> defaultLocale;
    localeHelp->GetDefault((ILocale**)&defaultLocale);

    AutoPtr<ILocaleDataHelper> ldh;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
    AutoPtr<ILocaleData> d;
    ldh->Get(defaultLocale, (ILocaleData**)&d);

    AutoPtr< ArrayOf<String> > names;
    switch (abbrev) {
        case IDateUtils::LENGTH_LONG:{
            d->GetLongMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_MEDIUM:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORT:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORTER:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORTEST:{
            d->GetTinyMonthNames((ArrayOf<String>**)&names);
            break;
        }
        default:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
    }
    if(names != NULL)
    {
        Int32 namesLen = names->GetLength();
        if( 0 <= month && month < namesLen ) {
            return (*names)[month];
        }
    }
    return String("");
}

String DateUtils::GetStandaloneMonthString(
    /* [in] */ Int32 month,
    /* [in] */ Int32 abbrev)
{
    // Note that here we use d.shortMonthNames for MEDIUM, SHORT and SHORTER.
    // This is a shortcut to not spam the translators with too many variations
    // of the same string.  If we find that in a language the distinction
    // is necessary, we can can add more without changing this API.
    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> defaultLocale;
    localeHelp->GetDefault((ILocale**)&defaultLocale);

    AutoPtr<ILocaleDataHelper> ldh;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
    AutoPtr<ILocaleData> d;
    ldh->Get(defaultLocale, (ILocaleData**)&d);

    AutoPtr< ArrayOf<String> > names;
    switch (abbrev) {
        case IDateUtils::LENGTH_LONG:{
            d->GetLongStandAloneMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_MEDIUM:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORT:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORTER:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
        case IDateUtils::LENGTH_SHORTEST:{
            d->GetTinyMonthNames((ArrayOf<String>**)&names);
            break;
        }
        default:{
            d->GetShortMonthNames((ArrayOf<String>**)&names);
            break;
        }
    }
    return (*names)[month];
}

AutoPtr<ICharSequence> DateUtils::GetRelativeTimeSpanString(
    /* [in] */ Int64 startTime)
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);
    return GetRelativeTimeSpanString(startTime, millis, IDateUtils::MINUTE_IN_MILLIS);
}

AutoPtr<ICharSequence> DateUtils::GetRelativeTimeSpanString(
        /* [in] */ Int64 time,
        /* [in] */ Int64 now,
        /* [in] */ Int64 minResolution)
{
    Int32 flags = IDateUtils::FORMAT_SHOW_DATE | IDateUtils::FORMAT_SHOW_YEAR | IDateUtils::FORMAT_ABBREV_MONTH;
    return GetRelativeTimeSpanString(time, now, minResolution, flags);
}

AutoPtr<ICharSequence> DateUtils::GetRelativeTimeSpanString(
    /* [in] */ Int64 time,
    /* [in] */ Int64 now,
    /* [in] */ Int64 minResolution,
    /* [in] */ Int32 flags)
{
    AutoPtr<IResources> r = CResources::GetSystem();
    Boolean abbrevRelative = ( (flags & (IDateUtils::FORMAT_ABBREV_RELATIVE | IDateUtils::FORMAT_ABBREV_ALL)) != 0 );

    Boolean past = (now >= time);
    Int64 duration = Elastos::Core::Math::Abs(now - time);

    Int32 resId;
    Int64 count;
    if ( (duration < IDateUtils::MINUTE_IN_MILLIS) && (minResolution < IDateUtils::MINUTE_IN_MILLIS) ) {
        count = duration / IDateUtils::SECOND_IN_MILLIS;
        if (past) {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_num_seconds_ago;
            }
            else {
                resId = R::plurals::num_seconds_ago;
            }
        }
        else {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_in_num_seconds;
            }
            else {
                resId = R::plurals::in_num_seconds;
            }
        }
    }
    else if ( (duration < IDateUtils::HOUR_IN_MILLIS) && (minResolution < IDateUtils::HOUR_IN_MILLIS) ) {
        count = duration / IDateUtils::MINUTE_IN_MILLIS;
        if (past) {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_num_minutes_ago;
            }
            else {
                resId = R::plurals::num_minutes_ago;
            }
        }
        else {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_in_num_minutes;
            }
            else {
                resId = R::plurals::in_num_minutes;
            }
        }
    }
    else if (duration < IDateUtils::DAY_IN_MILLIS && minResolution < IDateUtils::DAY_IN_MILLIS) {
        count = duration / IDateUtils::HOUR_IN_MILLIS;
        if (past) {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_num_hours_ago;
            }
            else {
                resId = R::plurals::num_hours_ago;
            }
        }
        else {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_in_num_hours;
            }
            else {
                resId = R::plurals::in_num_hours;
            }
        }
    }
    else if (duration < IDateUtils::WEEK_IN_MILLIS && minResolution < IDateUtils::WEEK_IN_MILLIS) {
        count = GetNumberOfDaysPassed(time, now);
        if (past) {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_num_days_ago;
            }
            else {
                resId = R::plurals::num_days_ago;
            }
        }
        else {
            if (abbrevRelative) {
                resId = R::plurals::abbrev_in_num_days;
            }
            else {
                resId = R::plurals::in_num_days;
            }
        }
    }
    else {
        // We know that we won't be showing the time, so it is safe to pass
        // in a null context.
        String fdrRet = FormatDateRange(NULL, time, time, flags);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(fdrRet, (ICharSequence**)&cs);
        return cs;
    }

    String format;
    r->GetQuantityString(resId, (Int32) count, &format);

    String retVal;
    retVal.AppendFormat(format.string(), count);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(retVal, (ICharSequence**)&cs);
    return cs;
}

Int64 DateUtils::GetNumberOfDaysPassed(
    /* [in] */ Int64 date1,
    /* [in] */ Int64 date2)
{
    Mutex::Autolock lock(sLockDateUtilsClass);
    if (sThenTime == NULL) {
        CTime::New((ITime**)&sThenTime);
    }
    Int64 thenTimeGmtoff;
    sThenTime->Set(date1);
    Int32 day1;
    sThenTime->GetGmtoff(&thenTimeGmtoff);
    day1 = CTime::GetJulianDay(date1, thenTimeGmtoff);
    sThenTime->Set(date2);
    Int32 day2;
    sThenTime->GetGmtoff(&thenTimeGmtoff);
    day2 = CTime::GetJulianDay(date2, thenTimeGmtoff);
    return Elastos::Core::Math::Abs(day2 - day1);
}

AutoPtr<ICharSequence> DateUtils::GetRelativeDateTimeString(
    /* [in] */ IContext* c,
    /* [in] */ Int64 time,
    /* [in] */ Int64 minResolution,
    /* [in] */ Int64 transitionResolution,
    /* [in] */ Int32 flags)
{
    AutoPtr<IResources> r = CResources::GetSystem();
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    Int64 now;
    system->GetCurrentTimeMillis(&now);
    Int64 duration = Elastos::Core::Math::Abs(now - time);

    // getRelativeTimeSpanString() doesn't correctly format relative dates
    // above a week or exact dates below a day, so clamp
    // transitionResolution as needed.
    if (transitionResolution > IDateUtils::WEEK_IN_MILLIS) {
        transitionResolution = IDateUtils::WEEK_IN_MILLIS;
    }
    else if (transitionResolution < IDateUtils::DAY_IN_MILLIS) {
        transitionResolution = IDateUtils::DAY_IN_MILLIS;
    }

    String fdr = FormatDateRange(c, time, time, IDateUtils::FORMAT_SHOW_TIME);
    AutoPtr<ICharSequence> timeClause;
    CStringWrapper::New(fdr, (ICharSequence**)&timeClause);

    String result;
    if (duration < transitionResolution) {
        AutoPtr<ICharSequence> relativeClause = GetRelativeTimeSpanString(time, now, minResolution, flags);
        String raw;
        r->GetString(R::string::relative_time, &raw);
        String strRelativeClause, strTimeClause;
        relativeClause->ToString(&strRelativeClause);
        timeClause->ToString(&strTimeClause);
//        result = String.format(raw, strRelativeClause, strTimeClause);
    }
    else {
        AutoPtr<ICharSequence> dateClause = GetRelativeTimeSpanString(c, time, FALSE);
        String raw;
        r->GetString(R::string::date_time, &raw);
        String strDateClause, strTimeClause;
        dateClause->ToString(&strDateClause);
        timeClause->ToString(&strTimeClause);
//        result = String.format(raw, strDateClause, strTimeClause);
    }

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(result, (ICharSequence**)&cs);
    return cs;
}

String DateUtils::GetRelativeDayString(
        /* [in] */ IResources* r,
        /* [in] */ Int64 day,
        /* [in] */ Int64 today)
{
    AutoPtr<ITime> startTime;
    CTime::New((ITime**)&startTime);
    startTime->Set(day);
    AutoPtr<ITime> currentTime;
    CTime::New((ITime**)&currentTime);
    currentTime->Set(today);

    Int64 startTimeGmtoff, currentTimeGmtoff;
    startTime->GetGmtoff(&startTimeGmtoff);
    currentTime->GetGmtoff(&currentTimeGmtoff);

    Int32 startDay = CTime::GetJulianDay(day, startTimeGmtoff);
    Int32 currentDay = CTime::GetJulianDay(today, currentTimeGmtoff);

    Int32 days = Elastos::Core::Math::Abs(currentDay - startDay);
    Boolean past = (today > day);

    // TODO: some locales name other days too, such as de_DE's "Vorgestern" (today - 2).
    AutoPtr<IConfiguration> configuration;
    r->GetConfiguration((IConfiguration**)&configuration);
    AutoPtr<ILocale> locale;
    configuration->GetLocale((ILocale**)&locale);
    if (locale == NULL) {
        AutoPtr<ILocaleHelper> localeHelp;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
        localeHelp->GetDefault((ILocale**)&locale);
    }

    AutoPtr<ILocaleData> localeDate;
    AutoPtr<ILocaleDataHelper> ldh;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
    ldh->Get(locale, (ILocaleData**)&localeDate);

    if (days == 1) {
        if (past) {
            String strYesterday;
            localeDate->GetYesterday(&strYesterday);
            return strYesterday;
        }
        else {
            String strTomorrow;
            localeDate->GetTomorrow(&strTomorrow);
            return strTomorrow;
        }
    }
    else if (days == 0) {
        String strToday;
        localeDate->GetToday(&strToday);
        return strToday;
    }

    Int32 resId;
    if (past) {
        resId = R::plurals::num_days_ago;
    }
    else {
        resId = R::plurals::in_num_days;
    }

    String format;
    r->GetQuantityString(resId, days, &format);

    String retVal;
//    retVal = String.format(format, days);
    return retVal;
}

ECode DateUtils::InitFormatStrings()
{
    Mutex::Autolock lock(sLock);
    InitFormatStringsLocked();
    return NOERROR;
}

ECode DateUtils::InitFormatStringsLocked()
{
    AutoPtr<IResources> r = CResources::GetSystem();
    AutoPtr<IConfiguration> cfg;
    r->GetConfiguration((IConfiguration**)&cfg);
    Boolean bLastConfigEquals;
    if (sLastConfig == NULL || (sLastConfig->Equals(cfg.Get(), &bLastConfigEquals), !bLastConfigEquals))
    {
       sLastConfig = cfg;

       AutoPtr<Elastos::Text::IDateFormatHelper> dfh;
       Elastos::Text::CDateFormatHelper::AcquireSingleton((Elastos::Text::IDateFormatHelper**)&dfh);
       sStatusTimeFormat = NULL;
       dfh->GetTimeInstanceEx(Elastos::Text::IDateFormat::SHORT, (Elastos::Text::IDateFormat**)&sStatusTimeFormat);

       r->GetString(R::string::elapsed_time_short_format_mm_ss, &sElapsedFormatMMSS);
       r->GetString(R::string::elapsed_time_short_format_h_mm_ss, &sElapsedFormatHMMSS);
    }
    return NOERROR;
}

AutoPtr<ICharSequence> DateUtils::TimeString(
    /* [in] */ Int64 millis)
{
    Mutex::Autolock lock(sLock);
    InitFormatStringsLocked();
    String ret;
//    sStatusTimeFormat->Format(millis, &ret);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(ret, (ICharSequence**)&cs);
    return cs;
}

AutoPtr<ICharSequence> DateUtils::FormatDuration(
    /* [in] */ Int64 millis)
{
    AutoPtr<IResourcesHelper> helper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&helper);
    AutoPtr<IResources> res;
    helper->GetSystem((IResources**)&res);
    String rStr;
    if (millis >= IDateUtils::HOUR_IN_MILLIS) {
        Int32 hours = (Int32) ((millis + 1800000) / IDateUtils::HOUR_IN_MILLIS);
        res->GetQuantityString(
                R::plurals::duration_hours, hours, &rStr);
    } else if (millis >= IDateUtils::MINUTE_IN_MILLIS) {
        Int32 minutes = (Int32) ((millis + 30000) / IDateUtils::MINUTE_IN_MILLIS);
        res->GetQuantityString(
                R::plurals::duration_minutes, minutes, &rStr);
    } else {
        Int32 seconds = (Int32) ((millis + 500) / IDateUtils::SECOND_IN_MILLIS);
        res->GetQuantityString(
                R::plurals::duration_seconds, seconds, &rStr);
    }
    AutoPtr<ICharSequence> rst;
    CStringWrapper::New(rStr, (ICharSequence**)&rst);
    return rst;
}

String DateUtils::FormatElapsedTime(
    /* [in] */ Int64 elapsedSeconds)
{
    return FormatElapsedTime(String(""), elapsedSeconds);
}

String DateUtils::FormatElapsedTime(
    /* [in] */ const String& recycle,
    /* [in] */ Int64 elapsedSeconds)
{
    InitFormatStrings();

    Int64 hours = 0;
    Int64 minutes = 0;
    Int64 seconds = 0;

    if (elapsedSeconds >= 3600) {
        hours = elapsedSeconds / 3600;
        elapsedSeconds -= hours * 3600;
    }
    if (elapsedSeconds >= 60) {
        minutes = elapsedSeconds / 60;
        elapsedSeconds -= minutes * 60;
    }
    seconds = elapsedSeconds;

    String result;
    if (hours > 0) {
        return FormatElapsedTime(recycle, sElapsedFormatHMMSS, hours, minutes, seconds);
    }
    else {
        return FormatElapsedTime(recycle, sElapsedFormatMMSS, minutes, seconds);
    }
}

ECode DateUtils::Append(
        /* [in,out] */ StringBuilder* sb,
        /* [in] */ Int64 value,
        /* [in] */  Boolean pad,
        /* [in] */  Char32 zeroDigit)
{
    if (value < 10) {
        if (pad) {
            sb->AppendChar(zeroDigit);
        }
    }
    else {
        Char32 c=(zeroDigit + (value / 10));
        sb->AppendChar(c);
    }
    Char32 c=(zeroDigit + (value % 10));

    sb->AppendChar(c);
    return NOERROR;
}

String DateUtils::FormatElapsedTime(
    /* [in] */ const String& recycle,
    /* [in] */ const String& format,
    /* [in] */ Int64 hours,
    /* [in] */ Int64 minutes,
    /* [in] */ Int64 seconds)
{
    if (FAST_FORMAT_HMMSS.Equals(format)) {

        AutoPtr<ILocaleHelper> localeHelp;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
        AutoPtr<ILocale> defaultLocale;
        localeHelp->GetDefault((ILocale**)&defaultLocale);

        AutoPtr<ILocaleDataHelper> ldh;
        CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
        AutoPtr<ILocaleData> localeDate;
        ldh->Get(defaultLocale, (ILocaleData**)&localeDate);

        Char32 zeroDigit;
        localeDate->GetZeroDigit(&zeroDigit);

        StringBuilder sb;
        sb.AppendString(recycle);
        Append(&sb, hours, FALSE, zeroDigit);
        sb.AppendChar(TIME_SEPARATOR);
        Append(&sb, minutes, TRUE, zeroDigit);
        sb.AppendChar(TIME_SEPARATOR);
        Append(&sb, seconds, TRUE, zeroDigit);
        return sb.ToString();
    }
    else {
        String retVal;
//        retVal = String.format(format, hours, minutes, seconds);
        return retVal;
    }
}

String DateUtils::FormatElapsedTime(
    /* [in] */ const String& recycle,
    /* [in] */ const String& format,
    /* [in] */ Int64 minutes,
    /* [in] */ Int64 seconds)
{
    if (FAST_FORMAT_MMSS.Equals(format)) {

        AutoPtr<ILocaleHelper> localeHelp;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
        AutoPtr<ILocale> defaultLocale;
        localeHelp->GetDefault((ILocale**)&defaultLocale);

        AutoPtr<ILocaleDataHelper> ldh;
        CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&ldh);
        AutoPtr<ILocaleData> localeDate;
        ldh->Get(defaultLocale, (ILocaleData**)&localeDate);

        Char32 zeroDigit;
        localeDate->GetZeroDigit(&zeroDigit);

        StringBuilder sb;
        sb.AppendString(recycle);
        Append(&sb, minutes, FALSE, zeroDigit);
        sb.AppendChar(TIME_SEPARATOR);
        Append(&sb, seconds, TRUE, zeroDigit);
        return sb.ToString();
    }
    else {
        String retVal;
//        retVal = String.format(format, minutes, seconds);
        return retVal;
    }
}

AutoPtr<ICharSequence> DateUtils::FormatSameDayTime(
    /* [in] */ Int64 then,
    /* [in] */ Int64 now,
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle)
{
    AutoPtr<ICalendar> thenCal;
    CGregorianCalendar::New((IGregorianCalendar**)&thenCal);
    thenCal->SetTimeInMillis(then);
    AutoPtr<IDate> thenDate;
    thenCal->GetTime((IDate**)&thenDate);
    AutoPtr<ICalendar> nowCal;
    CGregorianCalendar::New((IGregorianCalendar**)&nowCal);
    nowCal->SetTimeInMillis(now);

    AutoPtr<Elastos::Text::IDateFormat> f;

    Int32 thenYear,nowYear,thenMonth,nowMonth,thenDay,nowDay;
    thenCal->Get(ICalendar::YEAR, &thenYear);
    nowCal->Get(ICalendar::YEAR, &nowYear);
    thenCal->Get(ICalendar::MONTH, &thenMonth);
    nowCal->Get(ICalendar::MONTH, &nowMonth);
    thenCal->Get(ICalendar::DAY_OF_MONTH, &thenDay);
    nowCal->Get(ICalendar::DAY_OF_MONTH, &nowDay);

    AutoPtr<IDateFormatHelper> helper;
    CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&helper);
    if (thenYear == nowYear && thenMonth == nowMonth && thenDay == nowDay) {
        helper->GetTimeInstanceEx(timeStyle, (Elastos::Text::IDateFormat**)&f);
    }
    else {
        helper->GetDateInstanceEx(dateStyle, (Elastos::Text::IDateFormat**)&f);
    }

    String ret;
    f->FormatDate(thenDate, &ret);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(ret, (ICharSequence**)&cs);
    return cs;
}

AutoPtr<ICalendar> DateUtils::NewCalendar(
    /* [in] */ Boolean zulu)
{
    AutoPtr<ICalendar> calendar;
    AutoPtr<ICalendarHelper> ch;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&ch);
    if (zulu) {
        AutoPtr<ITimeZoneHelper> tzh;
        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
        AutoPtr<ITimeZone> timeZone;
        tzh->GetGMT((ITimeZone**)&timeZone);//GetTimeZone(String("GMT"), (ITimeZone**)&timeZone);
        ch->GetInstanceEx2(timeZone, (ICalendar**)&calendar);
        return calendar;
    }
    ch->GetInstance((ICalendar**)&calendar);
    return calendar;
}

Boolean DateUtils::IsToday(
    /* [in] */ Int64 when)
{
    AutoPtr<ITime> time;
    CTime::New((ITime**)&time);
    time->Set(when);

    Int32 thenYear,thenMonth,thenMonthDay;
    time->GetYear(&thenYear);
    time->GetMonth(&thenMonth);
    time->GetMonthDay(&thenMonthDay);

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);

    time->Set(millis);
    Int32 newYear, newMonth, newMonthDay;
    time->GetYear(&newYear);
    time->GetMonth(&newMonth);
    time->GetMonthDay(&newMonthDay);

    return (thenYear == newYear)
            && (thenMonth == newMonth)
            && (thenMonthDay == newMonthDay);
}

Boolean DateUtils::IsUTC(
    /* [in] */ const String& s)
{
    if (s.GetLength() == 16 && s.GetChar(15) == 'Z') {
        return TRUE;
    }
    if (s.GetLength() == 9 && s.GetChar(8) == 'Z') {
        // XXX not sure if this case possible/valid
        return TRUE;
    }
    return FALSE;
}

String DateUtils::WriteDateTime(
    /* [in] */ ICalendar* cal)
{
    AutoPtr<ITimeZone> tz;
    AutoPtr<ITimeZoneHelper> tzh;
    CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
    tzh->GetGMT((ITimeZone**)&tz);

    AutoPtr<IGregorianCalendar> c;
    CGregorianCalendar::New(tz, (IGregorianCalendar**)&c);
    Int64 timeInMillis;
    cal->GetTimeInMillis(&timeInMillis);
    c->SetTimeInMillis(timeInMillis);
    return WriteDateTime(c, TRUE);
}

String DateUtils::WriteDateTime(
    /* [in] */ ICalendar* cal,
    /* [in] */ Boolean zulu)
{
    StringBuilder sb;
    sb.EnsureCapacity(16);
    if (zulu) {
        sb.AppendCStr("               Z");    //Java:    sb.setLength(16);    sb.setCharAt(15, 'Z');
    }
    else {
        sb.AppendCStr("               ");//Java:    sb.setLength(15);
    }
    String strTemp = sb.ToString();
    return WriteDateTime(cal, &strTemp);
}

String DateUtils::WriteDateTime(
    /* [in] */ ICalendar* cal,
    /* [in,out] */ String* sb)
{
    Int32 n;
    StringBuilder sbTemp;
    sbTemp.AppendString(*sb);

    cal->Get(ICalendar::YEAR, &n);
    sbTemp.SetChar(3, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(2, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(1, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(0, (Char32)('0'+n%10));

    cal->Get(ICalendar::MONTH, &n);
    n = n + 1;
    sbTemp.SetChar(5, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(4, (Char32)('0'+n%10));

    cal->Get(ICalendar::DAY_OF_MONTH, &n);
    sbTemp.SetChar(7, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(6, (Char32)('0'+n%10));

    sbTemp.SetChar(8, 'T');

    cal->Get(ICalendar::HOUR_OF_DAY, &n);
    sbTemp.SetChar(10, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(9, (Char32)('0'+n%10));

    cal->Get(ICalendar::MINUTE, &n);
    sbTemp.SetChar(12, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(11, (Char32)('0'+n%10));

    cal->Get(ICalendar::SECOND, &n);
    sbTemp.SetChar(14, (Char32)('0'+n%10));
    n /= 10;
    sbTemp.SetChar(13, (Char32)('0'+n%10));

    *sb = sbTemp.ToString();

    return *sb;
}

ECode DateUtils::Assign(
    /* [in] */ ICalendar* lval,
    /* [in] */ ICalendar* rval)
{
    // there should be a faster way.
    lval->Clear();
    Int64 timeInMillis;
    rval->GetTimeInMillis(&timeInMillis);
    lval->SetTimeInMillis(timeInMillis);
    return NOERROR;
}

String DateUtils::FormatDateRange(
    /* [in] */ IContext* context,
    /* [in] */ Int64 startMillis,
    /* [in] */ Int64 endMillis,
    /* [in] */ Int32 flags)
{
    //Java:    Formatter f = new Formatter(new StringBuilder(50), Locale.getDefault());
    StringBuilder s(50);
    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> locale;
    localeHelp->GetDefault((ILocale**)&locale);
//    AutoPtr<Elastos::Utility::IFormatter> f;
//    CFormatter::New((char*)s, locale, (IFormatter**)&f);
//    AutoPtr<Elastos::Utility::IFormatter> fRet=FormatDateRange(context, f, startMillis, endMillis, flags);
    String ret;
//    fRet->ToString(&ret);
    return ret;
}

AutoPtr</*Elastos::Utility::IFormatter*/IInterface> DateUtils::FormatDateRange(
    /* [in] */ IContext* context,
    /* [in] */ /*Elastos::Utility::IFormatter*/IInterface* formatter,
    /* [in] */ Int64 startMillis,
    /* [in] */ Int64 endMillis,
    /* [in] */ Int32 flags)
{
    return FormatDateRange(context, formatter, startMillis, endMillis, flags, String(NULL));
}

AutoPtr</*Elastos::Utility::IFormatter*/IInterface> DateUtils::FormatDateRange(
    /* [in] */ IContext* context,
    /* [in] */ /*Elastos::Utility::IFormatter*/IInterface* formatter,
    /* [in] */ Int64 startMillis,
    /* [in] */ Int64 endMillis,
    /* [in] */ Int32 flags,
    /* [in] */ const String& timeZone)
{
    AutoPtr<IResources> res = CResources::GetSystem();
    Boolean showTime = (flags & IDateUtils::FORMAT_SHOW_TIME) != 0;
    Boolean showWeekDay = (flags & IDateUtils::FORMAT_SHOW_WEEKDAY) != 0;
    Boolean showYear = (flags & IDateUtils::FORMAT_SHOW_YEAR) != 0;
    Boolean noYear = (flags & IDateUtils::FORMAT_NO_YEAR) != 0;
    Boolean useUTC = (flags & IDateUtils::FORMAT_UTC) != 0;
    Boolean abbrevWeekDay = (flags & (IDateUtils::FORMAT_ABBREV_WEEKDAY | IDateUtils::FORMAT_ABBREV_ALL)) != 0;
    Boolean abbrevMonth = (flags & (IDateUtils::FORMAT_ABBREV_MONTH | IDateUtils::FORMAT_ABBREV_ALL)) != 0;
    Boolean noMonthDay = (flags & IDateUtils::FORMAT_NO_MONTH_DAY) != 0;
    Boolean numericDate = (flags & IDateUtils::FORMAT_NUMERIC_DATE) != 0;

    // If we're getting called with a single instant in time (from
    // e.g. formatDateTime(), below), then we can skip a lot of
    // computation below that'd otherwise be thrown out.
    Boolean isInstant = (startMillis == endMillis);

    AutoPtr<ITime> startDate;
    if (!timeZone.IsNullOrEmpty()) {
        CTime::New(timeZone, (ITime**)&startDate);
    }
    else if (useUTC) {
        CTime::New(ITime::TIMEZONE_UTC, (ITime**)&startDate);
    }
    else {
        CTime::New((ITime**)&startDate);
    }
    startDate->Set(startMillis);

    AutoPtr<ITime> endDate;
    Int32 dayDistance;
    if (isInstant) {
        endDate = startDate;
        dayDistance = 0;
    }
    else {
        if (!timeZone.IsNullOrEmpty()) {
            CTime::New(timeZone, (ITime**)&endDate);
        }
        else if (useUTC) {
            CTime::New(ITime::TIMEZONE_UTC, (ITime**)&endDate);
        }
        else {
            CTime::New( (ITime**)&endDate);
        }
        endDate->Set(endMillis);
        Int64 startDateGmtoff, endDateGmtoff;
        startDate->GetGmtoff(&startDateGmtoff);
        endDate->GetGmtoff(&endDateGmtoff);
        Int32 startJulianDay = CTime::GetJulianDay(startMillis, startDateGmtoff);
        Int32 endJulianDay = CTime::GetJulianDay(endMillis, endDateGmtoff);
        dayDistance = endJulianDay - startJulianDay;
    }

    Int32 endDateHour, endDateMinute, endDateSecond;
    if (!isInstant
        && ((endDate->GetHour(&endDateHour), endDateHour) | (endDate->GetMinute(&endDateMinute), endDateMinute) | (endDate->GetSecond(&endDateSecond), endDateSecond)) == 0
        && (!showTime || dayDistance <= 1)) {
        Int32 endDateMonthDay;
        endDate->GetMonthDay(&endDateMonthDay);
        endDate->SetMonthDay(endDateMonthDay-1);
        Int64 normalizeRet;
        endDate->Normalize(TRUE, &normalizeRet /* ignore isDst */);
    }

    Int32 startDay;
    Int32 startMonthNum;
    Int32 startYear;
    startDate->GetMonthDay(&startDay);
    startDate->GetMonth(&startMonthNum);
    startDate->GetYear(&startYear);

    Int32 endDay;
    Int32 endMonthNum;
    Int32 endYear;
    endDate->GetMonthDay(&endDay);
    endDate->GetMonth(&endMonthNum);
    endDate->GetYear(&endYear);

    String startWeekDayString;// = "";
    String endWeekDayString;// = "";
    if (showWeekDay) {
        String weekDayFormat;// = "";
        if (abbrevWeekDay) {
            weekDayFormat = IDateUtils::ABBREV_WEEKDAY_FORMAT;
        }
        else {
            weekDayFormat = IDateUtils::WEEKDAY_FORMAT;
        }
        startDate->Format(weekDayFormat, &startWeekDayString);
        if(isInstant){
            endWeekDayString = startWeekDayString;
        }
        else {
            endDate->Format(weekDayFormat, &endWeekDayString);
        }
    }

    String startTimeString;// = "";
    String endTimeString;// = "";
    if (showTime) {
        String startTimeFormat;// = "";
        String endTimeFormat;// = "";
        Boolean force24Hour = (flags & IDateUtils::FORMAT_24HOUR) != 0;
        Boolean force12Hour = (flags & IDateUtils::FORMAT_12HOUR) != 0;
        Boolean use24Hour;
        if (force24Hour) {
            use24Hour = TRUE;
        }
        else if (force12Hour) {
            use24Hour = FALSE;
        }
        else {
            use24Hour = DateFormat::Is24HourFormat(context);
        }
        if (use24Hour) {
            res->GetString(R::string::hour_minute_24, &startTimeFormat);
            startTimeFormat = endTimeFormat;
        }
        else {
            Boolean abbrevTime = (flags & (IDateUtils::FORMAT_ABBREV_TIME | IDateUtils::FORMAT_ABBREV_ALL)) != 0;
            Boolean capAMPM = (flags & IDateUtils::FORMAT_CAP_AMPM) != 0;
            Boolean noNoon = (flags & IDateUtils::FORMAT_NO_NOON) != 0;
            Boolean capNoon = (flags & IDateUtils::FORMAT_CAP_NOON) != 0;
            Boolean noMidnight = (flags & IDateUtils::FORMAT_NO_MIDNIGHT) != 0;
            Boolean capMidnight = (flags & IDateUtils::FORMAT_CAP_MIDNIGHT) != 0;

            Int32 startDateMinute, startDateSecond, endDateMinute, endDateSecond;
            startDate->GetMinute(&startDateMinute);
            startDate->GetSecond(&startDateSecond);
            endDate->GetMinute(&endDateMinute);
            endDate->GetSecond(&endDateSecond);
            Boolean startOnTheHour = startDateMinute == 0 && startDateSecond == 0;
            Boolean endOnTheHour = endDateMinute == 0 && endDateSecond == 0;
            if (abbrevTime && startOnTheHour) {
                if (capAMPM) {
                    res->GetString(R::string::hour_cap_ampm, &startTimeFormat);
                }
                else {
                    res->GetString(R::string::hour_ampm, &startTimeFormat);
                }
            }
            else {
                if (capAMPM) {
                    res->GetString(R::string::hour_minute_cap_ampm, &startTimeFormat);
                }
                else {
                    res->GetString(R::string::hour_minute_ampm, &startTimeFormat);
                }
            }

            // Don't waste time on setting endTimeFormat when
            // we're dealing with an instant, where we'll never
            // need the end point.  (It's the same as the start
            // point)
            if (!isInstant) {
                if (abbrevTime && endOnTheHour) {
                    if (capAMPM) {
                        res->GetString(R::string::hour_cap_ampm, &endTimeFormat);
                    }
                    else {
                        res->GetString(R::string::hour_ampm, &endTimeFormat);
                    }
                }
                else {
                    if (capAMPM) {
                        res->GetString(R::string::hour_minute_cap_ampm, &endTimeFormat);
                    }
                    else {
                        res->GetString(R::string::hour_minute_ampm, &endTimeFormat);
                    }
                }

                Int32 endDateHour;
                if ((endDate->GetHour(&endDateHour), endDateHour == 12) && endOnTheHour && !noNoon) {
                    if (capNoon) {
                        res->GetString(R::string::Noon, &endTimeFormat);
                    }
                    else {
                        res->GetString(R::string::noon, &endTimeFormat);
                    }
                }
                else if (endDateHour == 0 && endOnTheHour && !noMidnight) {
                    if (capMidnight) {
                        res->GetString(R::string::Midnight, &endTimeFormat);
                    }
                    else {
                        res->GetString(R::string::midnight, &endTimeFormat);
                    }
                }
            }

            Int32 startDateHour;
            if ((startDate->GetHour(&startDateHour), startDateHour == 12) && startOnTheHour && !noNoon) {
                if (capNoon) {
                    res->GetString(R::string::Noon, &startTimeFormat);
                }
                else {
                    res->GetString(R::string::noon, &startTimeFormat);
                }
                // Don't show the start time starting at midnight.  Show
                // 12am instead.
            }
        }

        startDate->Format(startTimeFormat, &startTimeString);
        if(isInstant){
            endTimeString = startTimeString;
        }
        else {
            endDate->Format(endTimeFormat, &endTimeString);
        }
    }

    // Show the year if the user specified FORMAT_SHOW_YEAR or if
    // the starting and end years are different from each other
    // or from the current year.  But don't show the year if the
    // user specified FORMAT_NO_YEAR.
    if (showYear) {
        // No code... just a comment for clarity.  Keep showYear
        // on, as they enabled it with FORMAT_SHOW_YEAR.  This
        // takes precedence over them setting FORMAT_NO_YEAR.
    }
    else if (noYear) {
        // They explicitly didn't want a year.
        showYear = FALSE;
    }
    else if (startYear != endYear) {
        showYear = TRUE;
    }
    else {
        // Show the year if it's not equal to the current year.
        AutoPtr<ITime> currentTime;
        CTime::New((ITime**)&currentTime);
        currentTime->SetToNow();
        Int32 currentYear;
        currentTime->GetYear(&currentYear);
        showYear = (startYear != currentYear);
    }

    String defaultDateFormat, fullFormat, dateRange;
    if (numericDate) {
        res->GetString(R::string::numeric_date, &defaultDateFormat);
    }
    else if (showYear) {
        if (abbrevMonth) {
            if (noMonthDay) {
                res->GetString(R::string::abbrev_month_year, &defaultDateFormat);
            }
            else {
                res->GetString(R::string::abbrev_month_day_year, &defaultDateFormat);
            }
        }
        else {
            if (noMonthDay) {
                res->GetString(R::string::month_year, &defaultDateFormat);
            }
            else {
                res->GetString(R::string::month_day_year, &defaultDateFormat);
            }
        }
    }
    else {
        if (abbrevMonth) {
            if (noMonthDay) {
                res->GetString(R::string::abbrev_month, &defaultDateFormat);
            }
            else {
                res->GetString(R::string::abbrev_month_day, &defaultDateFormat);
            }
        }
        else {
            if (noMonthDay) {
                res->GetString(R::string::month, &defaultDateFormat);
            }
            else {
                res->GetString(R::string::month_day, &defaultDateFormat);
            }
        }
    }

    if (showWeekDay) {
        if (showTime) {
            res->GetString(R::string::wday1_date1_time1_wday2_date2_time2, &fullFormat);
        }
        else {
            res->GetString(R::string::wday1_date1_wday2_date2, &fullFormat);
        }
    }
    else {
        if (showTime) {
            res->GetString(R::string::date1_time1_date2_time2, &fullFormat);
        }
        else {
            res->GetString(R::string::date1_date2, &fullFormat);
        }
    }

    if (noMonthDay && startMonthNum == endMonthNum && startYear == endYear) {
        // Example: "January, 2008"
        String strDefFormatStartDate;
        startDate->Format(defaultDateFormat, &strDefFormatStartDate);
        AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//        formatter->Format("%s", strDefFormatStartDate, (Elastos::Utility::IFormatter**)&ret);
        return ret;
    }

    if (startYear != endYear || noMonthDay) {
        // Different year or we are not showing the month day number.
        // Example: "December 31, 2007 - January 1, 2008"
        // Or: "January - February, 2008"
        String startDateString;
        startDate->Format(defaultDateFormat, &startDateString);
        String endDateString;
        endDate->Format(defaultDateFormat, &endDateString);

        // The values that are used in a fullFormat string are specified
        // by position.
        AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//        formatter->Format(fullFormat, startWeekDayString, startDateString, startTimeString, endWeekDayString, endDateString, endTimeString, (Elastos::Utility::IFormatter**)&ret);
        return ret;
    }

    // Get the month, day, and year strings for the start and end dates
    String monthFormat;
    if (numericDate) {
        monthFormat = IDateUtils::NUMERIC_MONTH_FORMAT;
    }
    else if (abbrevMonth) {
        res->GetString(R::string::short_format_month, &monthFormat);
    }
    else {
        monthFormat = IDateUtils::MONTH_FORMAT;
    }
    String startMonthString;
    startDate->Format(monthFormat, &startMonthString);
    String startMonthDayString;
    startDate->Format(IDateUtils::MONTH_DAY_FORMAT, &startMonthDayString);
    String startYearString;
    startDate->Format(IDateUtils::YEAR_FORMAT, &startYearString);

    String endMonthString;
    String endMonthDayString;
    String endYearString;
    if(isInstant){
        endMonthString=NULL;
        endMonthDayString=NULL;
        endYearString=NULL;
    }
    else {
        endDate->Format(monthFormat, &endMonthString);
        endDate->Format(IDateUtils::MONTH_DAY_FORMAT, &endMonthDayString);
        endDate->Format(IDateUtils::YEAR_FORMAT, &endYearString);
    }

    String startStandaloneMonthString = startMonthString;
    String endStandaloneMonthString = endMonthString;
    // We need standalone months for these strings in Persian (fa): http://b/6811327

    AutoPtr<ILocaleHelper> localeHelp;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelp);
    AutoPtr<ILocale> localeDef;
    localeHelp->GetDefault((ILocale**)&localeDef);

    String strLocaleDefLanguage;
    if (!numericDate && !abbrevMonth && (localeDef->GetLanguage(&strLocaleDefLanguage), strLocaleDefLanguage).Equals("fa")) {
        startDate->Format(String("%-B"), &startStandaloneMonthString);
        endDate->Format(String("%-B"), &endStandaloneMonthString);
    }

    if (startMonthNum != endMonthNum) {
        // Same year, different month.
        // Example: "October 28 - November 3"
        // or: "Wed, Oct 31 - Sat, Nov 3, 2007"
        // or: "Oct 31, 8am - Sat, Nov 3, 2007, 5pm"

        Int32 index = 0;
        if (showWeekDay) index = 1;
        if (showYear) index += 2;
        if (showTime) index += 4;
        if (numericDate) index += 8;
        Int32 resId = sSameYearTable[index];
        res->GetString(resId, &fullFormat);

        // The values that are used in a fullFormat string are specified
        // by position.
        AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//        formatter->Format(fullFormat, startWeekDayString, startMonthString, startMonthDayString, startYearString, startTimeString, endWeekDayString, endMonthString, endMonthDayString, endYearString, endTimeString, startStandaloneMonthString, endStandaloneMonthString, (Elastos::Utility::IFormatter**)&ret);

        return ret;
    }

    if (startDay != endDay) {
        // Same month, different day.
        Int32 index = 0;
        if (showWeekDay) index = 1;
        if (showYear) index += 2;
        if (showTime) index += 4;
        if (numericDate) index += 8;
        Int32 resId = sSameMonthTable[index];
        res->GetString(resId, &fullFormat);

        // The values that are used in a fullFormat string are specified
        // by position.
        AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//        formatter->Format(fullFormat, startWeekDayString, startMonthString, startMonthDayString, startYearString, startTimeString, endWeekDayString, endMonthString, endMonthDayString, endYearString, endTimeString, startStandaloneMonthString, endStandaloneMonthString, (Elastos::Utility::IFormatter**)&ret);
        return ret;
    }

    // Same start and end day
    Boolean showDate = (flags & IDateUtils::FORMAT_SHOW_DATE) != 0;

    // If nothing was specified, then show the date.
    if (!showTime && !showDate && !showWeekDay) showDate = TRUE;

    // Compute the time string (example: "10:00 - 11:00 am")
    String timeString;// = "";
    if (showTime) {
        // If the start and end time are the same, then just show the
        // start time.
        if (isInstant) {
            // Same start and end time.
            // Example: "10:15 AM"
            timeString = startTimeString;
        }
        else {
            // Example: "10:00 - 11:00 am"
            String timeFormat;
            res->GetString(R::string::time1_time2, &timeFormat);
            // Don't use the user supplied Formatter because the result will pollute the buffer.
            timeString = NULL;
//            timeString = String.format(timeFormat, startTimeString, endTimeString);
        }
    }

    // Figure out which full format to use.
    fullFormat = "";
    String dateString;// = "";
    if (showDate) {
        startDate->Format(defaultDateFormat, &dateString);
        if (showWeekDay) {
            if (showTime) {
                // Example: "10:00 - 11:00 am, Tue, Oct 9"
                res->GetString(R::string::time_wday_date, &fullFormat);
            }
            else {
                // Example: "Tue, Oct 9"
                res->GetString(R::string::wday_date, &fullFormat);
            }
        }
        else {
            if (showTime) {
                // Example: "10:00 - 11:00 am, Oct 9"
                res->GetString(R::string::time_date, &fullFormat);
            }
            else {
                // Example: "Oct 9"
                AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//                formatter->Format("%s", dateString, (Elastos::Utility::IFormatter**)&ret);
                return ret;
            }
        }
    }
    else if (showWeekDay) {
        if (showTime) {
            // Example: "10:00 - 11:00 am, Tue"
            res->GetString(R::string::time_wday, &fullFormat);
        }
        else {
            // Example: "Tue"
            AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//            formatter->Format("%s", startWeekDayString, (Elastos::Utility::IFormatter**)&ret);
            return ret;
        }
    }
    else if (showTime) {
        AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//        formatter->Format("%s", timeString, (Elastos::Utility::IFormatter**)&ret);
        return ret;
    }

    // The values that are used in a fullFormat string are specified
    // by position.
    AutoPtr</*Elastos::Utility::IFormatter*/IInterface> ret;
//    formatter->Format(fullFormat, timeString, startWeekDayString, dateString, (Elastos::Utility::IFormatter**)&ret);
    return ret;
}

String DateUtils::FormatDateTime(
    /* [in] */ IContext* context,
    /* [in] */ Int64 millis,
    /* [in] */ Int32 flags)
{
    return FormatDateRange(context, millis, millis, flags);
}

AutoPtr<ICharSequence> DateUtils::GetRelativeTimeSpanString(
    /* [in] */ IContext* c,
    /* [in] */ Int64 millis,
    /* [in] */ Boolean withPreposition)
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String result;
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    Int64 span = Elastos::Core::Math::Abs(now - millis);
    {
        Mutex::Autolock lock(sLockDateUtilsClass);
        if (sNowTime == NULL) {
            CTime::New( (ITime**)&sNowTime );
        }

        if (sThenTime == NULL) {
            CTime::New( (ITime**)&sThenTime );
        }

        sNowTime->Set(now);
        sThenTime->Set(millis);

        Int32 nowWeekDay, thenWeekDay, nowYear, thenYear;
        sNowTime->GetWeekDay(&nowWeekDay);
        sThenTime->GetWeekDay(&thenWeekDay);
        sNowTime->GetYear(&nowYear);
        sThenTime->GetYear(&thenYear);

        Int32 prepositionId;
        if (span < IDateUtils::DAY_IN_MILLIS && nowWeekDay == thenWeekDay) {
            // Same day
            Int32 flags = IDateUtils::FORMAT_SHOW_TIME;
            result = FormatDateRange(c, millis, millis, flags);
            prepositionId = R::string::preposition_for_time;
        }
        else if (nowYear != thenYear) {
            // Different years
            Int32 flags = IDateUtils::FORMAT_SHOW_DATE | IDateUtils::FORMAT_SHOW_YEAR | IDateUtils::FORMAT_NUMERIC_DATE;
            result = FormatDateRange(c, millis, millis, flags);

            // This is a date (like "10/31/2008" so use the date preposition)
            prepositionId = R::string::preposition_for_date;
        }
        else {
            // Default
            Int32 flags = IDateUtils::FORMAT_SHOW_DATE | IDateUtils::FORMAT_ABBREV_MONTH;
            result = FormatDateRange(c, millis, millis, flags);
            prepositionId = R::string::preposition_for_date;
        }
        if (withPreposition) {
            AutoPtr<IResources> res;
            c->GetResources((IResources**)&res);
            String raw;
            res->GetString(prepositionId, &raw);
            String strTemp;
//            strTemp = String.format(raw, result);
            result = strTemp;
        }
    }
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(result, (ICharSequence**)&cs);
    return cs;
}

AutoPtr<ICharSequence> DateUtils::GetRelativeTimeSpanString(
    /* [in] */ IContext* c,
    /* [in] */ Int64 millis)
{
    return GetRelativeTimeSpanString(c, millis, false /* no preposition */);
}

AutoPtr< ArrayOf<Int32> > DateUtils::GetSameMonthTable()
{
    AutoPtr< ArrayOf<Int32> > sameMt = ArrayOf<Int32>::Alloc(16);
    for(Int32 i=0; i<16; i++){
        (*sameMt)[i] = sSameMonthTable[i];
    }
    return sameMt;
}

AutoPtr< ArrayOf<Int32> > DateUtils::GetSameYearTable()
{
    AutoPtr< ArrayOf<Int32> > sameYt = ArrayOf<Int32>::Alloc(16);
    for(Int32 i=0; i<16; i++){
        (*sameYt)[i] = sSameYearTable[i];
    }
    return sameYt;
}


} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos
