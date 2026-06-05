//=========
// Clock.h
//=========

#pragma once


//=======
// Using
//=======

#include "Concurrency/Task.h"
#include "Timing/TimePoint.h"
#include "Global.h"


//===========
// Namespace
//===========

namespace Timing {


//=======
// Clock
//=======

class Clock: public Global<Clock>
{
public:
	// Using
	using Task=Concurrency::Task;

	// Friends
	friend Object;

	// Con-/Destructors
	static inline Handle<Clock> Create() { return Global::Create(); }

	// Common
	Event<Clock> Day;
	inline UINT GetDayOfMonth() { return m_Now.DayOfMonth; }
	inline UINT GetDayOfWeek() { return m_Now.DayOfWeek; }
	inline UINT GetDayOfYear() { return TimePoint::GetDayOfYear(m_Now); }
	inline UINT GetHour() { return m_Now.Hour; }
	inline UINT GetMinute() { return m_Now.Minute; }
	inline UINT GetMonth() { return m_Now.Month; }
	inline UINT GetSecond() { return m_Now.Second; }
	inline UINT GetYear() { return m_Now.Year; }
	Event<Clock> Hour;
	inline BOOL IsSet() { return m_Now.Year!=0; }
	Event<Clock> Minute;
	Event<Clock> Month;
	TIME_POINT const& Now();
	Event<Clock> Second;
	Event<Clock> Tick;
	Event<Clock> TimeSet;
	BOOL Update(TIME_POINT* TimePoint);
	Event<Clock> Year;

private:
	// Con-/Destructors
	Clock();

	// Common
	VOID ClockTask();
	VOID DoTick();
	TIME_POINT m_Before;
	Handle<Task> m_ClockTask;
	TIME_POINT m_Now;
	Handle<Clock> m_This;
};

}