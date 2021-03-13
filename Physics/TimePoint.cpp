//===============
// TimePoint.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include <time.h>
#include "Devices/Clock.h"
#include "TimePoint.h"

using namespace Culture;
using namespace Devices;


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
Struct->tm_hour=Time->Hour;
Struct->tm_isdst=0;
Struct->tm_mday=Time->DayOfMonth;
Struct->tm_min=Time->Minute;
Struct->tm_mon=Time->Month;
Struct->tm_sec=Time->Second;
Struct->tm_wday=Time->DayOfWeek;
Struct->tm_yday=0;
Struct->tm_year=Time->Year;
}


//========
// Struct
//========

VOID TimePointFromTickCount(TIMEPOINT* ptp, UINT64 uticks)
{
ptp->Year=0;
ptp->Month=(BYTE)(uticks>>40);
ptp->DayOfMonth=(BYTE)(uticks>>32);
ptp->DayOfWeek=(BYTE)(uticks>>24);
ptp->Hour=(BYTE)(uticks>>16);
ptp->Minute=(BYTE)(uticks>>8);
ptp->Second=(BYTE)uticks;
}

UINT64 TimePointToTickCount(TIMEPOINT const& tp)
{
if(tp.Year!=0)
	return 0;
UINT64 uticks=0;
uticks|=((UINT64)tp.Month)<<40;
uticks|=((UINT64)tp.DayOfMonth)<<32;
uticks|=((UINT64)tp.DayOfWeek)<<24;
uticks|=((UINT64)tp.Hour)<<16;
uticks|=((UINT64)tp.Minute)<<8;
uticks|=tp.Second;
return uticks;
}


//=========
// Strings
//=========

LPCSTR pStrDays[][7]={
	{ "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" },
	{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" }};

LPCSTR pStrMonths[][12]={
	{ "Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" },
	{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" }};


//==================
// Con-/Destructors
//==================

TimePoint::TimePoint():
TimePoint(nullptr, { 0, 0, 0, 0, 0, 0, 0 })
{}

TimePoint::TimePoint(TIMEPOINT const& tp):
TimePoint(nullptr, tp)
{}

TimePoint::TimePoint(Handle<String> hname):
TimePoint(hname, { 0, 0, 0, 0, 0, 0, 0 })
{}

TimePoint::TimePoint(Handle<String> hname, TIMEPOINT const& tp):
TypedVariable(hname, tp)
{
UpdateTimer();
}


//========
// Access
//========

UINT TimePoint::GetDayOfWeek(LPCSTR pday)
{
if(StringCompare(pday, "mon", 3, false)==0)
	return 1;
if(StringCompare(pday, "tue", 3, false)==0)
	return 2;
if(StringCompare(pday, "wed", 3, false)==0)
	return 3;
if(StringCompare(pday, "thu", 3, false)==0)
	return 4;
if(StringCompare(pday, "fri", 3, false)==0)
	return 5;
if(StringCompare(pday, "sat", 3, false)==0)
	return 6;
if(StringCompare(pday, "sun", 3, false)==0)
	return 7;
return 0;
}

UINT TimePoint::GetMonth(LPCSTR pmon)
{
if(StringCompare(pmon, "jan", 3, false)==0)
	return 1;
if(StringCompare(pmon, "feb", 3, false)==0)
	return 2;
if(StringCompare(pmon, "mar", 3, false)==0)
	return 3;
if(StringCompare(pmon, "apr", 3, false)==0)
	return 4;
if(StringCompare(pmon, "may", 3, false)==0)
	return 5;
if(StringCompare(pmon, "jun", 3, false)==0)
	return 6;
if(StringCompare(pmon, "jul", 3, false)==0)
	return 7;
if(StringCompare(pmon, "aug", 3, false)==0)
	return 8;
if(StringCompare(pmon, "sep", 3, false)==0)
	return 9;
if(StringCompare(pmon, "oct", 3, false)==0)
	return 10;
if(StringCompare(pmon, "nov", 3, false)==0)
	return 11;
if(StringCompare(pmon, "dec", 3, false)==0)
	return 12;
return 0;
}

BOOL TimePoint::IsAbsolute()
{
ScopedLock lock(cCriticalSection);
return tValue.Year!=0;
}

UINT64 TimePoint::ToSeconds()
{
ScopedLock lock(cCriticalSection);
return ToSeconds(tValue);
}

UINT64 TimePoint::ToSeconds(TIMEPOINT const& tp)
{
if(tp.Year==0)
	return 0;
if(tp.Year==0xFFFF)
	return -1;
tm t;
TimeStructFromTimePoint(&t, &tp);
/*WORD uyear4=uYear%4;
UINT uyear=uYear-uyear4;
UINT udays=uyear*365+uyear/4;
if(uyear4>0)
	udays+=366+(uyear4-1)*365;
UINT64 usec=udays*24*60*60;
usec+=DaysPerMonth[uyear4][uMonth-1]*24*60*60;
usec+=uHour*60*60;
usec+=uMinute*60;
usec+=uSecond;*/
return mktime(&t);
}

Handle<String> TimePoint::ToString()
{
ScopedLock lock(cCriticalSection);
TCHAR pstr[32];
ToString(tValue, pstr, 32, TimeFormat::Full);
return pstr;
}

Handle<String> TimePoint::ToString(TimeFormat fmt, LanguageCode lng)
{
ScopedLock lock(cCriticalSection);
TCHAR pstr[32];
ToString(tValue, pstr, 32, fmt, lng);
return pstr;
}

Handle<String> TimePoint::ToString(TIMEPOINT const& tp, TimeFormat fmt, LanguageCode lng)
{
TCHAR pstr[32];
ToString(tp, pstr, 32, fmt, lng);
return pstr;
}

UINT TimePoint::ToString(TIMEPOINT const& tp, LPTSTR pstr, UINT usize, TimeFormat fmt, LanguageCode lng)
{
ASSERT(pstr&&usize);
pstr[0]=0;
if(lng==LanguageCode::None)
	lng=Language::Current;
if(tp.Year==0)
	{
	UINT64 uticks=TimePointToTickCount(tp);
	if(uticks==0)
		return StringCopy(pstr, usize, "-");
	UINT64 uticksnow=GetTickCount64();
	UINT udelta=(UINT)((uticksnow-uticks)/1000);
	TCHAR ptimespan[32];
	TimeSpan::ToString(udelta, ptimespan, 32);
	switch(lng)
		{
		default:
		case LanguageCode::DE:
			return StringPrint(pstr, usize, "vor %s", ptimespan);
		case LanguageCode::US:
			return StringPrint(pstr, usize, "%s ago", ptimespan);
		}
	}
UINT ulng=(UINT)lng;
UINT uwday=tp.DayOfWeek;
UINT umday=tp.DayOfMonth;
UINT umon=tp.Month+1;
UINT uyear=tp.Year+1900;
UINT uhour=tp.Hour;
UINT umin=tp.Minute;
if(ulng>1||uwday>6||umon>12)
	return StringCopy(pstr, usize, "-");
switch(lng)
	{
	default:
	case LanguageCode::DE:
		{
		switch(fmt)
			{
			default:
			case TimeFormat::DateTime:
				return StringPrint(pstr, usize, "%02u.%02u.%u %02u:%02u", umday, umon, uyear, uhour, umin);
			case TimeFormat::Full:
				return StringPrint(pstr, usize, "%s, %u. %s %u %02u:%02u", pStrDays[ulng][uwday], umday, pStrMonths[ulng][umon-1], uyear, uhour, umin);
			case TimeFormat::Time:
				return StringPrint(pstr, usize, "%02u:%02u", uhour, umin);
			}
		break;
		}
	case LanguageCode::US:
		{
		switch(fmt)
			{
			default:
			case TimeFormat::DateTime:
				return StringPrint(pstr, usize, "%02u-%02u-%u %02u:%02u", umon, umday, uyear, uhour, umin);
			case TimeFormat::Full:
				return StringPrint(pstr, usize, "%s, %u %s %u - %02u:%02u", pStrDays[ulng][uwday], umday, pStrMonths[ulng][umon-1], uyear, uhour, umin);
			case TimeFormat::Time:
				return StringPrint(pstr, usize, "%02u:%02u", uhour, umin);
			}
		break;
		}
	}
return 0;
}

SIZE_T TimePoint::WriteToStream(Handle<OutputStream> hstream)
{
if(!hstream)
	return sizeof(TIMEPOINT);
ScopedLock lock(cCriticalSection);
TIMEPOINT tp(tValue);
if(tp.Year==0)
	ZeroMemory(&tp, sizeof(TIMEPOINT));
return hstream->Write(&tp, sizeof(TIMEPOINT));
}


//============
// Comparison
//============

BOOL TimePoint::operator==(TIMEPOINT const& tp)
{
ScopedLock lock(cCriticalSection);
return tValue==tp;
}


//==============
// Modification
//==============

VOID TimePoint::Clear()
{
TIMEPOINT tp;
ZeroMemory(&tp, sizeof(TIMEPOINT));
Set(tp);
}

BOOL TimePoint::FromTimeStamp(TIMEPOINT* ptp, LPCSTR pstr)
{
CHAR pday[4];
CHAR pmon[4];
UINT umday=0;
UINT uhour=0;
UINT umin=0;
UINT usec=0;
UINT uyear=0;
if(StringScan(pstr, "%s %s %u %u:%u:%u %u", pday, 4, pmon, 4, &umday, &uhour, &umin, &usec, &uyear)!=7)
	return false;
UINT uwday=GetDayOfWeek(pday);
if(uwday==0)
	return false;
if(uwday==7)
	uwday=0;
UINT umon=GetMonth(pmon);
if(umon==0)
	return false;
umon--;
uyear-=1900;
ptp->Second=(BYTE)usec;
ptp->Minute=(BYTE)umin;
ptp->Hour=(BYTE)uhour;
ptp->DayOfWeek=(BYTE)uwday;
ptp->DayOfMonth=(BYTE)umday;
ptp->Month=(BYTE)umon;
ptp->Year=(WORD)uyear;
return true;
}

VOID TimePoint::Set(TIMEPOINT const& tp)
{
TypedVariable::Set(tp);
UpdateTimer();
}


//================
// Common Private
//================

VOID TimePoint::OnChanged(Handle<Variable> hvar)
{
UpdateTimer();
}

VOID TimePoint::OnTimerTriggered(Handle<Timer> htimer)
{
if(Clock::Current->Update(&tValue))
	{
	hTimer->Stop();
	hTimer=nullptr;
	}
Changed(this);
}

VOID TimePoint::UpdateTimer()
{
UINT64 uticks=TimePointToTickCount(tValue);
if(uticks)
	{
	if(!hTimer)
		{
		hTimer=new Timer();
		hTimer->Triggered.Add(this, &TimePoint::OnTimerTriggered);
		hTimer->StartPeriodic(1000);
		}
	}
else
	{
	if(hTimer)
		{
		hTimer->Stop();
		hTimer=nullptr;
		}
	}
}

}