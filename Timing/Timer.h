//=========
// Timer.h
//=========

#pragma once


//=======
// Using
//=======

#include "Timing/Clock.h"


//===========
// Namespace
//===========

namespace Timing {


//=======
// Timer
//=======

class Timer: public Object
{
public:
	// Friends
	friend Object;

	// Con-/Destructors
	~Timer();
	static inline Handle<Timer> Create() { return Object::Create<Timer>(); }

	// Common
	BOOL IsStarted()const { return m_Interval!=0; }
	VOID Reset();
	VOID StartOnce(UINT MilliSeconds);
	VOID StartPeriodic(UINT MilliSeconds);
	VOID Stop();
	Event<Timer> Triggered;

private:
	// Con-/Destructors
	Timer();

	// Common
	VOID OnClockTick();
	INT m_Interval;
	SIZE_T m_NextTime;
	Handle<Clock> m_Clock;
};

}