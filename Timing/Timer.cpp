//===========
// Timer.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Devices/Timers/SystemTimer.h"
#include "Timer.h"

using namespace Devices::Timers;


//===========
// Namespace
//===========

namespace Timing {


//==================
// Con-/Destructors
//==================

Timer::~Timer()
{
if(m_Clock)
	m_Clock->Tick.Remove(this);
}


//========
// Common
//========

VOID Timer::Reset()
{
INT interval=m_Interval;
if(interval<0)
	interval*=-1;
m_NextTime=SystemTimer::GetTickCount()+interval;
}

VOID Timer::StartOnce(UINT ms)
{
if(m_Interval!=0)
	Stop();
m_Interval=ms;
m_NextTime=SystemTimer::GetTickCount()+ms;
m_Clock=Clock::Get();
m_Clock->Tick.Add(this, &Timer::OnClockTick);
}

VOID Timer::StartPeriodic(UINT ms)
{
if(m_Interval!=0)
	Stop();
m_Interval=-(INT)ms;
m_NextTime=SystemTimer::GetTickCount()+ms;
m_Clock=Clock::Get();
m_Clock->Tick.Add(this, &Timer::OnClockTick);
}

VOID Timer::Stop()
{
if(m_Clock)
	m_Clock->Tick.Remove(this);
m_Interval=0;
m_NextTime=0;
}


//==========================
// Con-/Destructors Private
//==========================

Timer::Timer():
m_Interval(0),
m_NextTime(0)
{}


//================
// Common Private
//================

VOID Timer::DoTrigger()
{
Triggered(this);
}

VOID Timer::OnClockTick()
{
SIZE_T now=SystemTimer::GetTickCount();
if(m_NextTime>now)
	return;
Triggered(this);
if(m_Interval>0)
	{
	Stop();
	return;
	}
UINT time=m_Interval>0? m_Interval: -m_Interval;
m_NextTime=now+time;
}

}