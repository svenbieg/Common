//=========
// Clock.h
//=========

#pragma once


//=======
// Using
//=======

#include "Devices/Timer.h"
#include "Physics/TimePoint.h"


//===========
// Namespace
//===========

namespace Devices {


//=======
// Clock
//=======

class Clock: public Object
{
private:
	// Using
	using TIMEPOINT=Physics::TIMEPOINT;
	using TimePoint=Physics::TimePoint;
	using Timer=Devices::Timer;

public:
	// Common
	Event<Clock> Day;
	static Handle<Clock> Get();
	Event<Clock> Hour;
	INT GetDayOfMonth()const { return cTimePoint.DayOfMonth; }
	INT GetDayOfYear()const;
	BOOL IsSet()const { return cTimePoint.Year!=0; }
	INT GetHour()const { return cTimePoint.Hour; }
	INT GetMinute()const { return cTimePoint.Minute; }
	INT GetMonth()const { return cTimePoint.Month+1; }
	INT GetYear()const { return cTimePoint.Year+1900; }
	Event<Clock> Minute;
	Event<Clock> Month;
	TIMEPOINT Now();
	Event<Clock> Second;
	Event<Clock> Tick;
	Event<Clock> TimeSet;
	BOOL Update(TIMEPOINT* TimePoint);
	Event<Clock> Year;

private:
	// Con-/Destructors
	Clock();

	// Common
	VOID DoSecond();
	VOID OnTimerTriggered();
	TIMEPOINT cTimePoint;
	static Handle<Clock> hCurrent;
	Handle<Timer> hTimer;
	UINT uTicks;
};

}