//=========
// Clock.h
//=========

#pragma once


//=======
// Using
//=======

#include "Core/Timer.h"
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
	using Timer=Core::Timer;

public:
	// Con-/Destructors
	Clock();

	// Common
	Event<Clock> EndOfDay;
	Event<Clock> EndOfYear;
	static Handle<Clock> Current;
	Event<Clock> Day;
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
	Event<Clock> TimeSet;
	BOOL Update(TIMEPOINT* TimePoint);
	Event<Clock> Year;

private:
	// Common
	VOID OnTimerTriggered(Handle<Timer> Timer);
	TIMEPOINT cTimePoint;
	Handle<Timer> hTimer;
};

}
