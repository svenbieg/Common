//===========
// Timer.cpp
//===========

#include "Timer.h"


//=======
// Using
//=======

#include "Devices/Timers/SystemTimer.h"

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
Stop();
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
m_Interval=ms;
m_NextTime=SystemTimer::GetTickCount()+ms;
if(!m_Clock)
	{
	m_Clock=Clock::Create();
	m_Clock->Tick.Add(this, &Timer::OnClockTick);
	}
}

VOID Timer::StartPeriodic(UINT ms)
{
m_Interval=-(INT)ms;
m_NextTime=SystemTimer::GetTickCount()+ms;
if(!m_Clock)
	{
	m_Clock=Clock::Create();
	m_Clock->Tick.Add(this, &Timer::OnClockTick);
	}
}

VOID Timer::Stop()
{
m_Interval=0;
m_NextTime=0;
if(m_Clock)
	{
	m_Clock->Tick.Remove(this);
	m_Clock=nullptr;
	}
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