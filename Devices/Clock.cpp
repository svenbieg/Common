//===========
// Clock.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Devices/ClockHelper.h"
#include "Clock.h"

using namespace Culture;
using namespace Physics;


//===========
// Namespace
//===========

namespace Devices {


//============
// Year-Table
//============

BYTE DaysPerMonth[4][12]={
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }};


//==================
// Con-/Destructors
//==================

Clock::Clock():
cTimePoint({ 0, 0, 0, 0, 0, 0, 0 })
{}


//========
// Common
//========

Handle<Clock> Clock::Current;

INT Clock::GetDayOfYear()const
{
if(cTimePoint.Year==0)
	return 0;
UINT uyear=cTimePoint.Year;
WORD uyear4=uyear%4;
INT iday=0;
for(INT i=0; i<cTimePoint.Month; i++)
	iday+=DaysPerMonth[uyear4][i];
iday+=cTimePoint.DayOfMonth+1;
return iday;
}

TIMEPOINT Clock::Now()
{
if(!hTimer)
	{
	hTimer=new Timer();
	hTimer->Triggered.Add(this, &Clock::OnTimerTriggered);
	hTimer->StartPeriodic(1000);
	}
TIMEPOINT tp;
if(!ClockGetTime(tp))
	{
	UINT64 uticks=GetTickCount64();
	TimePointFromTickCount(&tp, uticks);
	}
return tp;
}

BOOL Clock::Update(TIMEPOINT* ptp)
{
if(!ptp)
	return false;
if(cTimePoint.Year==0)
	return false;
UINT64 uticks=TimePointToTickCount(*ptp);
if(uticks==0)
	return false;
UINT64 uticksnow=GetTickCount64();
UINT udelta=(UINT)((uticksnow-uticks)/1000);
return ClockGetTime(*ptp, udelta);
}


//================
// Common Private
//================

VOID Clock::OnTimerTriggered(Handle<Timer> htimer)
{
TIMEPOINT tpold(cTimePoint);
TIMEPOINT tpnew;
if(!ClockGetTime(tpnew))
	return;
if(tpold.Year==0)
	{
	cTimePoint=tpnew;
	TimeSet(this);
	return;
	}
if(tpold.DayOfMonth!=tpnew.DayOfMonth)
	EndOfDay(this);
if(tpold.Year!=tpnew.Year)
	EndOfYear(this);
cTimePoint=tpnew;
if(tpold.Year!=tpnew.Year)
	Year(this);
if(tpold.Month!=tpnew.Month)
	Month(this);
if(tpold.DayOfMonth!=tpnew.DayOfMonth)
	Day(this);
if(tpold.Hour!=tpnew.Hour)
	Hour(this);
if(tpold.Minute!=tpnew.Minute)
	Minute(this);
}

}
