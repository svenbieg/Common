//===============
// TimePoint.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include <time.h>
#include "Devices/Clock.h"
#include "Resources/Strings/Days.h"
#include "Resources/Strings/Months.h"
#include "Sentence.h"
#include "TimePoint.h"

using namespace Devices;
using namespace Resources::Strings;


//===========
// Namespace
//===========

namespace Physics {


//==================
// Helper-Functions
//==================

VOID TimeStructFromTimePoint(tm* Struct, TIMEPOINT const* Time)
{
if(Time->Year==0)
	{
	ZeroMemory(Struct, sizeof(tm));
	return;
	}
UINT wday=Time->DayOfWeek;
if(wday==7)
	wday=0;
Struct->tm_hour=Time->Hour;
Struct->tm_isdst=0;
Struct->tm_mday=Time->DayOfMonth;
Struct->tm_min=Time->Minute;
Struct->tm_mon=Time->Month;
Struct->tm_sec=Time->Second;
Struct->tm_wday=wday;
Struct->tm_yday=0;
Struct->tm_year=Time->Year;
}


//========
// Struct
//========

VOID TimePointFromTickCount(TIMEPOINT* tp, UINT64 ticks)
{
tp->Year=0;
tp->Month=(BYTE)(ticks>>40);
tp->DayOfMonth=(BYTE)(ticks>>32);
tp->DayOfWeek=(BYTE)(ticks>>24);
tp->Hour=(BYTE)(ticks>>16);
tp->Minute=(BYTE)(ticks>>8);
tp->Second=(BYTE)ticks;
}

UINT64 TimePointToTickCount(TIMEPOINT const& tp)
{
if(tp.Year!=0)
	return 0;
UINT64 ticks=0;
ticks|=((UINT64)tp.Month)<<40;
ticks|=((UINT64)tp.DayOfMonth)<<32;
ticks|=((UINT64)tp.DayOfWeek)<<24;
ticks|=((UINT64)tp.Hour)<<16;
ticks|=((UINT64)tp.Minute)<<8;
ticks|=tp.Second;
return ticks;
}


//==================
// Con-/Destructors
//==================

TimePoint::TimePoint():
TimePoint(nullptr, { 0, 0, 0, 0, 0, 0, 0 })
{}

TimePoint::TimePoint(TIMEPOINT const& tp):
TimePoint(nullptr, tp)
{}

TimePoint::TimePoint(Handle<String> name):
TimePoint(name, { 0, 0, 0, 0, 0, 0, 0 })
{}

TimePoint::TimePoint(Handle<String> name, TIMEPOINT const& tp):
TypedVariable(name, tp)
{
UpdateTimer();
}


//========
// Access
//========

UINT TimePoint::GetDayOfWeek(LPCSTR str)
{
auto days=STRS_DAYS;
for(UINT u=0; u<7; u++)
	{
	LPCSTR day=Translate(days[u], LNG::EN);
	if(StringCompare(str, day, 3, false)==0)
		return u+1;
	}
return 0;
}

UINT TimePoint::GetMonth(LPCSTR str)
{
auto months=STRS_MONTHS;
for(UINT u=0; u<12; u++)
	{
	LPCSTR month=Translate(months[u], LNG::EN);
	if(StringCompare(str, month, 3, false)==0)
		return u+1;
	}
return 0;
}

BOOL TimePoint::IsAbsolute()
{
SharedLock lock(cMutex);
return tValue.Year!=0;
}

UINT64 TimePoint::ToSeconds()
{
SharedLock lock(cMutex);
return ToSeconds(tValue);
}

UINT64 TimePoint::ToSeconds(TIMEPOINT const& tp)
{
if(tp.Year==0)
	return 0;
if(tp.Year==0xFFFF)
	return 0;
tm t;
TimeStructFromTimePoint(&t, &tp);
/*WORD uyear4=uYear%4;
UINT year=uYear-uyear4;
UINT udays=year*365+year/4;
if(uyear4>0)
	udays+=366+(uyear4-1)*365;
UINT64 sec=udays*24*60*60;
sec+=DaysPerMonth[uyear4][uMonth-1]*24*60*60;
sec+=uHour*60*60;
sec+=uMinute*60;
sec+=uSecond;*/
return mktime(&t);
}

Handle<String> TimePoint::ToString(LanguageCode lng)
{
SharedLock lock(cMutex);
CHAR str[64];
ToString(tValue, str, 64, TimeFormat::DateTime, lng);
return str;
}

Handle<String> TimePoint::ToString(TimeFormat fmt, LanguageCode lng)
{
SharedLock lock(cMutex);
CHAR str[64];
ToString(tValue, str, 64, fmt, lng);
return str;
}

Handle<String> TimePoint::ToString(TIMEPOINT const& tp, TimeFormat fmt, LanguageCode lng)
{
CHAR str[64];
ToString(tp, str, 64, fmt, lng);
return str;
}

UINT TimePoint::ToString(TIMEPOINT const& tp, LPSTR str, UINT size, TimeFormat fmt, LanguageCode lng)
{
if(!str||!size)
	return 0;
str[0]=0;
if(tp.Year==0)
	{
	UINT64 ticks=TimePointToTickCount(tp);
	if(ticks==0)
		return StringCopy(str, size, "-");
	return ToStringRelative(ticks, str, size, fmt, lng);
	}
switch(fmt)
	{
	default:
	case TimeFormat::DateTime:
		return ToStringDateTime(tp, str, size, lng);
	case TimeFormat::Full:
		return ToStringFull(tp, str, size, lng);
	case TimeFormat::Time:
		return ToStringTime(tp, str, size, lng);
	}
return 0;
}

SIZE_T TimePoint::WriteToStream(OutputStream* stream)
{
if(!stream)
	return sizeof(TIMEPOINT);
SharedLock lock(cMutex);
TIMEPOINT tp(tValue);
if(tp.Year==0)
	ZeroMemory(&tp, sizeof(TIMEPOINT));
return stream->Write(&tp, sizeof(TIMEPOINT));
}


//============
// Comparison
//============

BOOL TimePoint::operator==(TIMEPOINT const& tp)
{
SharedLock lock(cMutex);
return tValue==tp;
}


//==============
// Modification
//==============

VOID TimePoint::Clear(BOOL notify)
{
TIMEPOINT tp;
ZeroMemory(&tp, sizeof(TIMEPOINT));
Set(tp, notify);
}

BOOL TimePoint::FromTimeStamp(TIMEPOINT* tp, LPCSTR str)
{
CHAR day_str[4];
CHAR mon_str[4];
UINT mday=0;
UINT hour=0;
UINT min=0;
UINT sec=0;
UINT year=0;
if(StringScan(str, "%s %s %u %u:%u:%u %u", day_str, 4, mon_str, 4, &mday, &hour, &min, &sec, &year)!=7)
	return false;
UINT wday=GetDayOfWeek(day_str);
if(wday==0)
	return false;
UINT mon=GetMonth(mon_str);
if(mon==0)
	return false;
year-=1900;
tp->Second=(BYTE)sec;
tp->Minute=(BYTE)min;
tp->Hour=(BYTE)hour;
tp->DayOfWeek=(BYTE)wday;
tp->DayOfMonth=(BYTE)mday;
tp->Month=(BYTE)mon;
tp->Year=(WORD)year;
return true;
}

VOID TimePoint::Set(TIMEPOINT const& tp, BOOL notify)
{
TypedVariable::Set(tp, notify);
UpdateTimer();
}


//================
// Common Private
//================

VOID TimePoint::OnClockSecond(Clock* clock)
{
UniqueLock lock(cMutex);
if(!clock->Update(&tValue))
	return;
clock->Second.Remove(this);
lock.Unlock();
Changed(this);
}

UINT TimePoint::ToStringDateTime(TIMEPOINT const& tp, LPSTR str, UINT size, LanguageCode lng)
{
if(!str||!size)
	return 0;
str[0]=0;
UINT mday=tp.DayOfMonth;
UINT mon=tp.Month+1;
UINT year=tp.Year+1900;
UINT hour=tp.Hour;
UINT min=tp.Minute;
switch(lng)
	{
	case LNG::None:
	case LNG::DE:
		return StringPrint(str, size, "%02u.%02u.%u %02u:%02u", mday, mon, year, hour, min);
	case LNG::EN:
		return StringPrint(str, size, "%02u-%02u-%u %02u:%02u", mon, mday, year, hour, min);
	}
return 0;
}

UINT TimePoint::ToStringFull(TIMEPOINT const& tp, LPSTR str, UINT size, LanguageCode lng)
{
if(!str||!size)
	return 0;
str[0]=0;
UINT year=tp.Year+1900;
UINT mon=tp.Month+1;
UINT mday=tp.DayOfMonth;
UINT wday=tp.DayOfWeek;
UINT hour=tp.Hour;
UINT min=tp.Minute;
if(wday==0||wday>7||mon==0||mon>12)
	return StringCopy(str, size, "-");
auto str_mon=Translate(STRS_MONTHS[mon-1], lng);
auto str_day=Translate(STRS_DAYS[wday-1], lng);
switch(lng)
	{
	case LNG::None:
	case LNG::DE:
		return StringPrint(str, size, "%s, %u. %s %u %02u:%02u", str_day, mday, str_mon, year, hour, min);
	case LNG::EN:
		return StringPrint(str, size, "%s, %u %s %u - %02u:%02u", str_day, mday, str_mon, year, hour, min);
	}
return 0;
}

UINT TimePoint::ToStringRelative(UINT64 ticks, LPSTR str, UINT size, TimeFormat fmt, LanguageCode lng)
{
if(!str||!size)
	return 0;
str[0]=0;
UINT64 ticks_now=GetTickCount64();
UINT delta=(UINT)((ticks_now-ticks)/1000);
CHAR time_span[32];
TimeSpan::ToString(delta, time_span, 32);
UINT len=0;
switch(lng)
	{
	case LNG::None:
	case LNG::DE:
		return StringPrint(str, size, "vor %s", time_span);
	case LNG::EN:
		return StringPrint(str, size, "%s ago", time_span);
	}
return 0;
}

UINT TimePoint::ToStringTime(TIMEPOINT const& tp, LPSTR str, UINT size, LanguageCode lng)
{
UINT hour=tp.Hour;
UINT min=tp.Minute;
return StringPrint(str, size, "%02u:%02u", hour, min);
}

VOID TimePoint::UpdateTimer()
{
UniqueLock lock(cMutex);
auto clock=Clock::Get();
clock->Second.Remove(this);
UINT64 ticks=TimePointToTickCount(tValue);
if(ticks)
	clock->Second.Add(this, &TimePoint::OnClockSecond);
}

}